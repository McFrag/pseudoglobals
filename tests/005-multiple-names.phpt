--TEST--
multiple configured pseudoglobals are registered
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--INI--
pseudoglobals.register=_T, _CFG,	_AUTH
--FILE--
<?php
$_T = 't';
$_CFG = 'cfg';
$_AUTH = 'auth';

function test_multiple()
{
    echo $_T, PHP_EOL;
    echo $_CFG, PHP_EOL;
    echo $_AUTH, PHP_EOL;
}

test_multiple();
?>
--EXPECT--
t
cfg
auth
