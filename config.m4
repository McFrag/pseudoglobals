PHP_ARG_ENABLE(pseudoglobals,
    whether to enable pseudoglobals,
[
  --enable-pseudoglobals Enable pseudoglobals
])

if test "$PHP_PSEUDOGLOBALS" != "no"; then
    PHP_NEW_EXTENSION(pseudoglobals, src/pseudoglobals.c, $ext_shared)
fi
