<?php
/*
 * 
*/
class index extends controller{
	public function __construct() {
		parent::__construct();
	}
	public function index() {
		$data=new stdClass();
		$data->hello='hello world';
		$this->view('index/index.php',$data);
	}
}