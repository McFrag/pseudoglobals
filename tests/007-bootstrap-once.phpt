--TEST--
pseudoglobals bootstrap executes during request initialization
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.register=_T,_CFG
pseudoglobals.bootstrap=tests/fixtures/bootstrap-once.php
--FILE--
<?php
var_dump(isset($GLOBALS['bootstrap_count']));
echo $_T['enter'], PHP_EOL;
echo $_CFG['mode'], PHP_EOL;
echo $GLOBALS['bootstrap_count'], PHP_EOL;
?>
--EXPECT--
bool(true)
Enter
test
1
