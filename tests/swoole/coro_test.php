<?php
use Swoole\Coroutine\Http\Client;
use function Swoole\Coroutine\run;
use Swoole\Coroutine;

$h = new WandaHook();
$r = go(function (){
var_dump(333);
});
