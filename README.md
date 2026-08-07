# pseudoglobals

`pseudoglobals` is a small PHP extension that lets an application register its
own Zend auto-globals and initialize them from a PHP bootstrap file.

```php
echo "<button>{$_T['save']}</button>";
```

No `global $_T`, singleton accessor, or custom template syntax is required.

## Version 0.1.1

Version 0.1.1 moves bootstrap execution from the auto-global JIT callback to
PHP request initialization (`RINIT`).

This fixes initialization under Apache with OPcache, where cached op-arrays can
bypass compilation and therefore skip compile-time JIT callbacks on later
requests.

Pseudoglobal names are still registered once at module startup, while the
bootstrap now executes exactly once for every request.

## Configuration

Global PHP configuration:

```ini
extension=pseudoglobals.so
pseudoglobals.register=_T,_CFG,_AUTH
```

`pseudoglobals.register` is `PHP_INI_SYSTEM`, because names are registered in
`MINIT`.

`pseudoglobals.bootstrap` is `PHP_INI_PERDIR`, so Apache virtual hosts can
select their own bootstrap:

```apache
<VirtualHost *:80>
    ServerName example.test

    php_admin_value pseudoglobals.bootstrap \
        "/srv/www/example/pseudoglobals_init.php"
</VirtualHost>
```

A global default can also be set:

```ini
pseudoglobals.bootstrap=/srv/www/default/pseudoglobals_init.php
```

## Bootstrap

```php
<?php

$_T = new Translator();
$_CFG = Config::instance();
$_AUTH = Auth::current();
```

The bootstrap must initialize every configured pseudoglobal.

## Lifecycle

```text
PHP process starts
    MINIT
        register configured names as Zend auto-globals

request starts
    per-directory / virtual-host PHP configuration is applied
    RINIT
        execute pseudoglobals.bootstrap
        initialize configured variables

application script executes
    $_T, $_CFG, ... are already available
```

This behavior is independent of whether the application script is freshly
compiled or served from OPcache.

## Build

PHP 8.2 example:

```sh
phpize8.2 --clean
phpize8.2

./configure \
    --enable-pseudoglobals \
    --with-php-config=/usr/bin/php-config8.2

make
make test
```

For PHP 7.4, use the corresponding `phpize7.4` and `php-config7.4`.

## License

MIT
