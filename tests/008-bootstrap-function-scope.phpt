--TEST--
bootstrap initializes a pseudoglobal first accessed inside a function
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.names=_T
pseudoglobals.init_file=tests/fixtures/bootstrap-function.php
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
