--TEST--
pseudoglobals exposes its INI settings
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.names=_T,_CFG,_AUTH
pseudoglobals.init_file=/tmp/init_pseudoglobals.php
--FILE--
<?php
echo ini_get('pseudoglobals.names'), PHP_EOL;
echo ini_get('pseudoglobals.init_file'), PHP_EOL;
?>
--EXPECT--
_T,_CFG,_AUTH
/tmp/init_pseudoglobals.php
