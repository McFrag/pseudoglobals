--TEST--
pseudoglobals configuration is system-level
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.names=_T
--FILE--
<?php
var_dump(ini_set('pseudoglobals.names', '_OTHER'));
echo ini_get('pseudoglobals.names'), PHP_EOL;
?>
--EXPECT--
bool(false)
_T
