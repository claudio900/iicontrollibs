<?php
/*
$Id$
ownet.php

VERSION: 2006.12.20 - 12:44  BRST

Copyright (c) 2006 Spadim Technology / Brazil. All rights reserved.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
	
OWFS is an open source project developed by Paul Alfille and hosted at
http://www.owfs.org

 mailto:roberto@spadim.com.br
	www.spadim.com.br



To test this example you could start owserver with a fake-adapter, and
device 10.67C6697351FF will be available at port 1234.

# owserver --fake 10 -p 1234
# php ownet_example.php
*/


//check to make sure the file exists
if(!function_exists('bcadd'))  {
  if(file_exists("/opt/owfs/bin/bcadd.php"))  {
    require "/opt/owfs/bin/bcadd.php";
  } else if(file_exists("bcadd.php"))  {
    require "bcadd.php";
  } else  {
    die("File 'bcadd.php' is not found.\n");
  }
}

//check to make sure the file exists
if(file_exists("/opt/owfs/bin/ownet.php"))  {
  require "/opt/owfs/bin/ownet.php";
} else if(file_exists("ownet.php"))  {
  require "ownet.php";
} else {
  die("File 'ownet.php' is not found.\n");
}


$ow=new OWNet("tcp://127.0.0.1:1234");
var_dump($ow->get("/",OWNET_MSG_DIR,true));
var_dump($ow->get("/10.67C6697351FF/temperature",OWNET_MSG_READ,true));
var_dump($ow->get("/10.67C6697351FF",OWNET_MSG_PRESENCE,true));
var_dump($ow->get("/WRONG VALUE",OWNET_MSG_PRESENCE,true));

var_dump($ow->get("/",OWNET_MSG_DIR,false));
var_dump($ow->get("/10.67C6697351FF/temperature",OWNET_MSG_READ,false));
var_dump($ow->get("/10.67C6697351FF",OWNET_MSG_PRESENCE,false));
var_dump($ow->get("/WRONG VALUE",OWNET_MSG_PRESENCE,false));

?>
