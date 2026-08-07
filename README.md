# pseudoglobals

`pseudoglobals` is a small PHP extension that lets an application register its
own Zend auto-globals and initialize them from a PHP bootstrap file.

It exists for cases where native PHP variable syntax is desirable everywhere,
including inside functions and string interpolation:

```php
echo "<button>{$_T['save']}</button>";
```

No `global $_T`, singleton accessor, custom template syntax, or string
preprocessor is required.

## Status

Version 0.1.0 is the first release candidate. PHP 7.4 is the original
development target.

## Configuration

```ini
extension=pseudoglobals.so

pseudoglobals.register=_T,_CFG,_AUTH
pseudoglobals.bootstrap=/srv/www/init_pseudoglobals.php
```

`pseudoglobals.register` is a comma-separated list of variable names.
Names must start with `_`.

`pseudoglobals.bootstrap` is a PHP file executed once per request when PHP first
recognizes a registered pseudoglobal.

Both directives are `PHP_INI_SYSTEM`.

## Bootstrap

```php
<?php

$_T = new Translator();
$_CFG = Config::instance();
$_AUTH = Auth::current();
```

The bootstrap must initialize every registered pseudoglobal. If no bootstrap
is configured, application PHP code may initialize the variables itself.

## Semantics

Registration uses `zend_register_auto_global()` with JIT enabled. A reference
to a configured pseudoglobal can therefore trigger initialization while PHP is
compiling the containing script, before that script's first statement runs.

The bootstrap may itself assign registered pseudoglobals. PHP 7.4 can invoke
the JIT callback again while compiling the bootstrap; the extension treats that
as expected re-entry and disarms that callback rather than recursively executing
the bootstrap.

## Build

For PHP 7.4:

```sh
phpize7.4
./configure --enable-pseudoglobals \
    --with-php-config=/usr/bin/php-config7.4
make
make test
```

## Source layout

```text
src/module.c                  module lifecycle and phpinfo()
src/config.c                  INI configuration
src/registry.c                name parsing and auto-global registration
src/bootstrap.c               bootstrap execution and JIT callback
src/pseudoglobals_internal.h  internal declarations
```

## Tests

The PHPT suite covers loading, INI configuration, function-scope visibility,
multiple names, invalid names, one-shot bootstrap execution, and bootstrap use
from function scope.

```sh
make test
```

## License

MIT
