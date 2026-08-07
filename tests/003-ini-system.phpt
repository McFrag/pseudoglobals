--TEST--
pseudoglobals configuration is system-level
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.register=_T
--FILE--
<?php
var_dump(ini_set('pseudoglobals.register', '_OTHER'));
echo ini_get('pseudoglobals.register'), PHP_EOL;
?>
--EXPECT--
bool(false)
_T
