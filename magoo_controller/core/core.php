<?php
/*
 * Project: Magoo
 * Engineer: Makin
 * Time: 2014-6-17 23:54:59
 */
class core {
	protected $config;
	public function config() {
		$this->config=include 'config/config_index.php';//引入主配置文件
	}
	public function lang(){
		return include $this->config->lang;
	}
	protected function get($str=false,$request=false){
		$A=$request?$_POST:$_GET;
		if($str){
			return htmlspecialchars($A[$str], ENT_QUOTES);
		}else{			
			return (object)$A;
		}
	}
	protected function post($str=false){
		return $this->get($str,true);
	}
	protected function RandString($x=0){
			$r='abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ-_!';
			for($i=0;$i<$x;$i++){
				$echo.=$r[rand(0,strlen($r)-1)];
			}
			$time=substr(str_replace('.','',microtime(true)),0,12);
			return str_shuffle($time.$echo);
	}
	protected function view($template,$data=false,$memcache=false) {
		$templatefile=$this->config->path.$this->config->view.$template;
		#开启ob缓存
		if($memcache){ob_start();}
		is_file($templatefile)?require $templatefile:$this->notfound('view_not_found');
		if($memcache){
			$this->memcached();
		}
	}
	protected function location($url='/'){
		header("location: $url");
	}
	public function init() {
		header("program: magoo");
		$this->config();
		$index=$this->get('MaGooType')?$this->get('MaGooType'):$this->config->index;
		$expindex=explode('/', $index);
		$controller=$this->config->path.$this->config->controller.$this->config->controller_name_pre.$expindex[0].'.php';
		$this->class=$expindex[0];
		$this->method=$expindex[1]?$expindex[1]:$this->config->index;
		if(is_file($controller)&&!$expindex[2]){
			return $controller;
		}else{
			$this->notfound('controller_not_found');
		}	
	}
	public function notfound($info=''){
		header("HTTP/1.0 404 Not Found");
		header("Status: 404 Not Found");
		if($this->config->notexists){
			if(is_file($this->config->notexists)){
				require $this->config->notexists;
				exit;
			}else{
				exit($info?$this->lang()->$info:$this->lang()->file_exists);
			}
		}else{
			exit($controller."\n".$this->lang()->controller_exists);
		}
	}
	protected function memcached(){
		$mem = new Memcache;
		$mem->connect($this->memcache->Host,$this->memcache->Port) or die ($this->memcache->Connfail);
		$this->mem=$mem;
	}
}
