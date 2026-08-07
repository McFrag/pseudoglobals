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
pseudoglobals.init_file=/tmp/init.php
--FILE--
<?php
var_dump(ini_set('pseudoglobals.names', '_OTHER'));
var_dump(ini_set('pseudoglobals.init_file', '/tmp/other.php'));
echo ini_get('pseudoglobals.names'), "\n";
echo ini_get('pseudoglobals.init_file'), "\n";
?>
--EXPECT--
bool(false)
bool(false)
_T
/tmp/init.php
