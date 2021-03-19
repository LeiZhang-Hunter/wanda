--TEST--
Check if wanda is loaded
--SKIPIF--
<?php
if (!extension_loaded('wanda')) {
	echo 'skip';
}
/*
$string = "http://www.baidu.com?a=1";
$ch = curl_init();
curl_setopt($ch,CURLOPT_URL, $string);
curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);
$r = curl_exec($ch);
var_dump(curl_getinfo($ch));
*/
var_dump(file_get_contents("http://www.baidu.com"));
?>
--FILE--
<?php
echo 'The extension "wanda" is available';
?>
--EXPECT--
The extension "wanda" is available
