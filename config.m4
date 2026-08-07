PHP_ARG_ENABLE(pseudoglobals,
    whether to enable pseudoglobals,
[  --enable-pseudoglobals   Enable pseudoglobals support])

if test "$PHP_PSEUDOGLOBALS" != "no"; then
    PHP_NEW_EXTENSION(pseudoglobals, src/module.c src/config.c src/registry.c src/bootstrap.c, $ext_shared)
fi
