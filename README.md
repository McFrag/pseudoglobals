# pseudoglobals

Application-defined PHP auto-globals.

## Commit 4

This commit adds lazy bootstrap execution.

Configuration:

```ini
pseudoglobals.names=_T,_CFG,_AUTH
pseudoglobals.init_file=/srv/www/init_pseudoglobals.php
```

Configured names are registered as JIT auto-globals. The first access to any
configured pseudoglobal executes `pseudoglobals.init_file`. The bootstrap runs
once per request and must initialize every configured pseudoglobal.

Example bootstrap:

```php
<?php

$_T = new Translator();
$_CFG = Config::instance();
$_AUTH = Auth::current();
```

Application code can then use them directly from any function scope:

```php
function renderButton()
{
    echo "<button>{$_T['save']}</button>";
}
```

If `pseudoglobals.init_file` is empty, the extension does not bootstrap
anything; applications may initialize registered pseudoglobals themselves.

## Build for PHP 7.4

```sh
phpize7.4
./configure --enable-pseudoglobals --with-php-config=/usr/bin/php-config7.4
make
make test
```
