<?

$WebEngine_version="2.64";

$WebEngine_host="localhost:/var/run/mysqld/mysqld.sock";

$WebEngine_database="la21nl2002";
$WebEngine_database="la21nl2002";

$WebEngine_admin_user="la21nl2002RO";

$WebEngine_admin_password="jjjrjje993mmdsjdjj";
$WebEngine_database="la21nl2003";

$WebEngine_admin_user="la21nl2003RO";

$WebEngine_admin_password="dfjrrnrnn5nrjsd78s";


$Webengine_include_path="/usr/local/webengine/" . $WebEngine_version . "/";

$Webengine_local_inc = $Webengine_include_path . "local.inc";

$dblink=mysql_connect($WebEngine_host,$WebEngine_admin_user,$WebEngine_admin_password);

?>