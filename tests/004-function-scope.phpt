--TEST--
configured pseudoglobals are visible in function scope
--SKIPIF--
<?php if (!extension_loaded('pseudoglobals')) die('skip pseudoglobals extension not loaded'); ?>
--INI--
pseudoglobals.register=_T
pseudoglobals.bootstrap=
--FILE--
<?php $_T=['enter'=>'Enter']; function t(){var_dump(isset($_T)); echo $_T['enter'],PHP_EOL;} t(); ?>
--EXPECT--
bool(true)
Enter
