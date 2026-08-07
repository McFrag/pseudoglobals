--TEST--
pseudoglobals extension loads
--SKIPIF--
<?php
if (!extension_loaded('pseudoglobals')) {
    die('skip pseudoglobals extension not loaded');
}
?>
--FILE--
<?php
echo extension_loaded('pseudoglobals') ? "loaded\n" : "not loaded\n";
echo phpversion('pseudoglobals'), "\n";
?>
--EXPECT--
loaded
0.2.0
