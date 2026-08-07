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

Version **0.1.0** is the first release candidate. The CI matrix targets PHP
7.4 through PHP 8.4. PHP 7.4 is the original development target.

## Configuration

```ini
extension=pseudoglobals.so

pseudoglobals.register=_T,_CFG,_AUTH
pseudoglobals.bootstrap=/srv/www/init_pseudoglobals.php
```

`pseudoglobals.register` is a comma-separated list of variable names. Names
must start with `_`.

`pseudoglobals.bootstrap` is a PHP file executed once per request when PHP first
recognizes a registered pseudoglobal.

Both directives are `PHP_INI_SYSTEM`, so they can be configured globally or
per Apache/FPM configuration where system-level PHP directives are allowed.

## Bootstrap

```php
<?php

$_T = new Translator();
$_CFG = Config::instance();
$_AUTH = Auth::current();
```

The bootstrap must initialize every registered pseudoglobal. If no bootstrap is
configured, application PHP code may initialize the variables itself.

## Semantics

Registration uses `zend_register_auto_global()` with JIT enabled. A reference
to a configured pseudoglobal can therefore trigger initialization while PHP is
**compiling** the containing script, before the script's first statement runs.

For example, if a file contains `$_T` anywhere, the bootstrap may already have
run by the time the first executable statement in that file is reached.

The bootstrap may itself assign registered pseudoglobals. PHP can invoke the
JIT callback again while compiling the bootstrap; the extension treats that as
expected re-entry and disarms the callback rather than recursively executing
the bootstrap.

## Build

```sh
phpize
./configure --enable-pseudoglobals
make
make test
```

For an installation with several PHP versions, use the matching `phpize` and
`php-config`, for example:

```sh
phpize7.4
./configure --enable-pseudoglobals \
    --with-php-config=/usr/bin/php-config7.4
make
make test
```

## Source layout

```text
src/module.c       module lifecycle and phpinfo()
src/config.c       INI configuration
src/registry.c     name parsing and auto-global registration
src/bootstrap.c    bootstrap execution and JIT callback
```

Internal helpers are declared in `src/pseudoglobals_internal.h`, including:

```c
pseudoglobals_is_registered()
pseudoglobals_registered_count()
pseudoglobals_initialize()
pseudoglobals_register_configured()
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
