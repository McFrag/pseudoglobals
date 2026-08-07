--TEST--
pseudoglobals configuration modes
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.register=_T
pseudoglobals.bootstrap=
--FILE--
<?php
var_dump(ini_set('pseudoglobals.register', '_OTHER'));
var_dump(ini_set('pseudoglobals.bootstrap', '/tmp/other.php'));
echo ini_get('pseudoglobals.register'), PHP_EOL;
echo ini_get('pseudoglobals.bootstrap'), PHP_EOL;
?>
--EXPECT--
bool(false)
bool(false)
_T
