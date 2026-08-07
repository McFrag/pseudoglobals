# pseudoglobals

Version 0.1.1 moves bootstrap execution to RINIT so it runs once per request even when OPcache reuses compiled scripts.

Global registration:
```ini
pseudoglobals.register=_T,_CFG,_AUTH
```

Per-vhost bootstrap:
```apache
php_admin_value pseudoglobals.bootstrap "/srv/www/example/pseudoglobals_init.php"
```

`pseudoglobals.register` is PHP_INI_SYSTEM. `pseudoglobals.bootstrap` is PHP_INI_PERDIR.
