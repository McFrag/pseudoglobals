# pseudoglobals

Application-defined PHP auto-globals.

## Current state

This commit introduces extension configuration and request initialization state.
It does **not** register pseudoglobals yet.

### Configuration

```ini
extension=pseudoglobals.so
pseudoglobals.names=_T,_CFG,_AUTH
pseudoglobals.init_file=/srv/www/init_pseudoglobals.php
```

Both settings are `PHP_INI_SYSTEM`, so they can be configured per server/vhost
where the PHP SAPI permits system-level configuration.

## Build (PHP 7.4)

```sh
phpize
./configure --enable-pseudoglobals
make
```

For a machine with several PHP versions, use the PHP 7.4 development tools
explicitly, for example:

```sh
phpize7.4
./configure --enable-pseudoglobals --with-php-config=/usr/bin/php-config7.4
make
```

Run the tests with:

```sh
make test
```

The next commit will parse `pseudoglobals.names` and register those names with
`zend_register_auto_global()`.
