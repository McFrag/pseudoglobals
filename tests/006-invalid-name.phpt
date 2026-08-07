--TEST--
invalid pseudoglobal names are ignored with a warning
--SKIPIF--
<?php if (!extension_loaded('pseudoglobals')) die('skip pseudoglobals extension not loaded'); ?>
--INI--
pseudoglobals.register=_T,BAD
pseudoglobals.bootstrap=
--FILE--
<?php $_T='ok'; function t(){echo $_T,PHP_EOL;} t(); ?>
--EXPECTF--
PHP Warning:  PHP Startup: Ignoring pseudoglobal "BAD": name must begin with '_' in Unknown on line 0

Warning: PHP Startup: Ignoring pseudoglobal "BAD": name must begin with '_' in Unknown on line 0
ok
