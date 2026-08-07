--TEST--
multiple configured pseudoglobals are registered
--SKIPIF--
<?php if (!extension_loaded('pseudoglobals')) die('skip pseudoglobals extension not loaded'); ?>
--INI--
pseudoglobals.register=_T, _CFG,	_AUTH
pseudoglobals.bootstrap=
--FILE--
<?php $_T='t';$_CFG='cfg';$_AUTH='auth'; function t(){echo $_T,PHP_EOL,$_CFG,PHP_EOL,$_AUTH,PHP_EOL;} t(); ?>
--EXPECT--
t
cfg
auth
