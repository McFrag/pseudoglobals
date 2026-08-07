--TEST--
bootstrap initializes a pseudoglobal recognized inside a function
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.register=_T
pseudoglobals.bootstrap=tests/fixtures/bootstrap-function.php
--FILE--
<?php
function demo()
{
    echo $_T['enter'], PHP_EOL;
}

demo();
?>
--EXPECT--
Enter
