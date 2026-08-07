# pseudoglobals

Application-defined PHP auto-globals.

## Commit 3

This commit adds registration of configured pseudoglobals.

Configuration:

```ini
pseudoglobals.names=_T,_CFG,_AUTH
pseudoglobals.init_file=/srv/www/init_pseudoglobals.php
```

`pseudoglobals.names` is parsed during module initialization. Each valid name is
registered with Zend using `zend_register_auto_global()`, which makes the
variable visible in function scope without a `global` declaration.

Example:

```php
$_T = ['enter' => 'Enter'];

function demo()
{
    echo $_T['enter'];
}
```

The bootstrap file is not executed yet. That will be introduced in Commit #4.

## Build

For PHP 7.4:

```sh
phpize7.4
./configure --enable-pseudoglobals --with-php-config=/usr/bin/php-config7.4
make
make test
```
