<?php
/*
 * Project: Magoo
 * Engineer: Makin
 * Time: 2014-6-1 17:39:35
 * 
 */
#配置核心目录文件夹名称
$corename='core/';
require_once $corename.'core.php';//引入核心文件
require $corename.'controller.php';//引入主控制器

$core=new core;
$controller=$core->init();//初始化内核

require $controller;//引入分级控制器文件 （比如：controller_xxxx.php）

$index=new $core->class;//实例化分级控制器
#检查类中是否存在该方法
if(!method_exists($index,$core->method)){
    $core->notfound('method_not_found');
}
#判断类中的方法是否公开
$refl = new ReflectionMethod($index,$core->method);
if(!$refl->isPublic()){
	$core->notfound('method_not_public');
}
#引入数据库连接池文件
require $corename.'db/conn.php';
#引入数据库配置文件
$db->config=require $corename.'config/config_db.php';
$index->memcache=require $corename.'config/config_memcache.php';
$index->db=$db;//数据源配置给分级控制器
$method=$core->method;
$index->$method();//调用分级控制器中的方法