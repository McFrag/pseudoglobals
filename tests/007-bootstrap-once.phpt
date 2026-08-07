--TEST--
pseudoglobals bootstrap executes once on first access
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.names=_T,_CFG
pseudoglobals.init_file=tests/fixtures/bootstrap-once.php
--FILE--
<?php
var_dump(isset($GLOBALS['bootstrap_count']));

echo $_T['enter'], PHP_EOL;
echo $_CFG['mode'], PHP_EOL;

echo $GLOBALS['bootstrap_count'], PHP_EOL;
?>
--EXPECT--
bool(false)
Enter
test
1
