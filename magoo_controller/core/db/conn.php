<?php
/*
 * Engineer:Makin
 * Project: conn
 * Time: 2014-4-29 17:00:26
 * ->DBHost;#数据库服务器地址
 * ->DBRoot;#数据库服务器账户
 * ->DBPawd;#数据库服务器密码
 * ->DBName;#数据库服务器名称
 * ->DBPre; #数据库数据表前缀
 */
class Conn{
	public $config;
	protected $mysqli;
	private function Mysql(){
		if($this->mysqli){return false;}
		$this->mysqli = @new mysqli($this->config->DBHost,$this->config->DBRoot,$this->config->DBPawd,$this->config->DBName);	
		$this->mysqli->connect_error?header("Status: 303 Not Found"):0;
		$this->mysqli->query("SET NAMES 'UTF8'");
	}
	#table assembly
	public function P($t){
		return $this->config->DBPre?$this->config->DBPre.'_'.$t:$t;
	}
	public function HtmlChars($string){
		return htmlspecialchars($string,ENT_QUOTES);
	}
	#data
	public function ArrData($arr){
		if(!is_array($arr)&&!is_object($arr)){return $arr;}
		if(is_object($arr)){
			$arr=(array)$arr;
		}
		if(get_magic_quotes_gpc()){
			foreach($arr as $k=>$v){
				$data.=($data?',`':'`').$k."`='".$this->HtmlChars(stripslashes($v))."'";
			}
		}else{
			foreach($arr as $k=>$v){
				$data.=($data?',`':'`').$k."`='".$this->HtmlChars($v)."'";
			}
		}
		return $data;
	}
	#query
	public function Query($string){
		$this->Mysql();
		return $this->mysqli->query($string);
	}
	#assoc
	public function Assoc($query,$bool=false,$add=false){
		if($bool){
			while($a=@$query->fetch_assoc()){$arr[]=(object)($add?array_merge($a, $add):$a);}
		}else{
			$a=@$query->fetch_assoc();
			$arr=$a?(object)($add?array_merge($a, $add):$a):false; 
		}
		return $arr?$arr:array();
	}
	#assoc_query
	public function Assoc_Query($select,$bool=false,$add=false){
		return $this->Assoc($this->Query($select),$bool,$add);
	}
	#读取
	public function Select($a,$t,$w=false){
		return $this->Query("select $a from `".$this->P($t)."` $w");
	}
	#总数
	public function SelectNum($t,$w="",$a='count(*) as num') {
		return $this->SelectOne($a,$t,$w)->num;
	}
	#单条
	public function SelectOne($a,$t,$w=""){
		return $this->Assoc($this->Select($a,$t,$w));
	}
	#所有
	public function SelectAll($a,$t,$w="",$add=false) {
		$q=$this->Select($a,$t,$w);
		return $this->Assoc($q,true,$add);
	}
	#插入
	public function Insert($t,$arr,$w=""){
		return $this->Query("insert into `".$this->P($t)."` set ".$this->ArrData($arr)." $w");
	}
	#更改
	public function Update($t,$arr,$w=""){
		return $this->Query("update `".$this->P($t)."` set".$this->ArrData($arr)." $w");
	}
	#删除
	public function Delete($t,$w){
		return $this->Query("delete from `".$this->P($t)."` $w");
	}
	#数目
	public function Num_rows($query){
		return $query->num_rows;
	}
	#获取插入ID
	public function Getinsertid() {
		return $this->mysqli->insert_id;
	}
	#query操作影响的数目
	public function Rows(){
		return $this->mysqli->affected_rows;
	}
	#检查表
	public function Exists_Table($t){
		return $this->Assoc($this->Query("show tables like '".$this->P($t)."'"));
		//return $this->Assoc($this->Query("select `TABLE_NAME` from `INFORMATION_SCHEMA`.`TABLES` where     `TABLE_NAME`='".$this->P($t)."'"))->TABLE_NAME;
	}
	#优化表
	public function Optimize($t) {
		return $this->Query("OPTIMIZE TABLE $this->P($t)");
	}
	#重新排序主键
	public function ReorderKey($t){
		if($this->Query("ALTER TABLE `".$this->P($t)."` DROP `id`")){
			return $this->Query("ALTER TABLE `".$this->P($t)."` ADD `id` INT NOT NULL AUTO_INCREMENT PRIMARY KEY FIRST");
		}		
	}
	#ajax
	public function Ajax(){
		$G=(object)$_GET;
		$a=$G->action=='select'||$G->action=='selectone'||$G->action=='selectall'?1:0;
		$return=$this->$_GET['action']($G->field?$G->field:($a?"*":$G->table),$_POST?$_POST:$G->table,$G->where);
		return $return?$a?$G->json?json_encode($return):$return:$return:'';
	}
}
$db=new Conn;