<?php
/*
 * Project: Magoo
 * Engineer: Makin
 * Time: 2014-6-28 23:14:17
 * Frame profile
 */
return (object)array(
	#应用程序目录
	'path'=>'Application/'
	#控制器目录
	,'controller'=>'controller/'
	#视图目录
	,'view'=>'view/'
	#控制器文件名前缀
	,'controller_name_pre'=>'controller_'
	#控制器默认方法名
	,'index'=>'index'
	#语言文件路径
	,'lang'=>'lang/chinese.php'
	#自定义404地址 ,不填写起调试作用
	,'notexists'=>'plug/404.html'
	#过滤器。定义需要过滤跳转到的页面
	,'filter'=>'http://xxx.com/xxx/%E5%8A%A8%E4%BD%9C.html'
	#后台登录session_id的名称
	,'session_id_name'=>'bt_user_id'
	#cookie domain
	,'cookie_domain'=>'btba.com.cn'
	#定义程序静态URL地址
	,'http_index'=>'http://www.godcode.com.cn/'
	,'info'=>(object)array(
		'sitename'=>'BT吧'
		,'sitefullname'=>'BT吧-全球最大的BT种子下载站 (BTBA-CHINA)'
		,'siteurl'=>'www.btba.com.cn'
		,'header_name'=>''//
		,'qq_group'=>''
		,'header_right_ad'=>''
		,'count'=>''
        ,'ad'=>''
	)
	#图片上传服务器地址(ip/domain)
	,'imglist'=>array('http://img0.wcsz.com.cn/')//127.0.0.1:100 http://211.149.190.43:801/img0.coutuan.com
	#文件服务器地址(ip/domain)
	,'filelist'=>array('http://btba.com.cn/')//127.0.0.1:600
    #缓存服务器地址(ip/domain)
	,'cacheurl'=>array('127.0.0.1:100')//127.0.0.1:10
    #后台服务器地址(ip/domain)必填，与后台通信使用
	,'adminurl'=>array('http://btba.com.cn/')
);