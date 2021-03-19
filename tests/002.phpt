--TEST--
wanda_test1() Basic test
--SKIPIF--
<?php
if (!extension_loaded('wanda')) {
	echo 'skip';
}
?>
--FILE--
<?php
$ret = wanda_test1();

var_dump($ret);
?>
--EXPECT--
The extension wanda is loaded and working!
NULL
