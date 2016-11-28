<?

if ( ! get_magic_quotes_gpc() ):
	echo "Magic quotes Get/Post/Cookie are off. Refusing to continue...";
	exit;
endif;
if ( ini_get( 'magic_quotes_sybase' ) ):
	echo "Magic quotes for sybase are on. Refusing to continue...";
	exit;
endif;

	
$settings = new stdClass; 

$settings->parent_url = "http://www.duurzaamheidsmeter.nl";
$settings->site_url = "http://www.duurzaamheidsmeter.nl/resultaten";

$settings->stylesheet = "duurzaam.css";

$settings->db_host = "127.0.0.1:3306";
$settings->db_readonly_user = "ROEva1Oove";
$settings->db_readonly_password = "PhaiH3ia";
$settings->db_link = mysql_connect( $settings->db_host,
                                    $settings->db_readonly_user,
                                    $settings->db_readonly_password );
	if ( ! $settings->db_link ):
		die ( "Could not connect to database: " . mysql_error() );
	endif;

$settings->db_name = "la21nl2007";
$settings->db_select = mysql_select_db( $settings->db_name );
	if ( ! $settings->db_select ):
		die ( "Could not select database '{$settings->db_name}': " . mysql_error() );
	endif;

$settings->map_file = "duurzaam2007.map";

$settings->tooltips = "<script language=\"JavaScript\" type=\"text/javascript\" src=\"{$settings->site_url}/wz_tooltip.js\"></script>";
$settings->tooltips_on = "no";

// years
	$years = array( "2007" );
	$default_year = "2007";
// languages
	$languages = array( "nl", "en" );
	$languages_long = array( "nl"=>"nederlands", "en"=>"english" );
	$languages_longer = array( "nl"=>"Nederlands", "en"=>"English" );
	$default_language = "nl";
// colors for the maps
	$color_unknown = "F9EBC7";
	$color_gradient_start = "FFFFFE";
	$color_gradient_end = "396CAE";
	// vragen
	$colors_vragen = array(	"onbekend" 	=> $color_unknown,
									"ja"			=> "00CC99",
									"nee"			=> "FF9999" );
// load php_mapscript
//	dl('php_mapscript.so');
// form fields
	$invuller_formfields = array( "invuller_naam"=>"Naam (verplicht)",
											"invuller_organisatie"=>"Organisatie of afdeling",
											"invuller_adres"=>"Adres",
											"invuller_postcode"=>"Postcode",
											"invuller_plaats"=>"Plaats",
											"invuller_tel"=>"Telefoon (verplicht)",
											"invuller_fax"=>"Fax",
											"invuller_email"=>"E-mail",
											"invuller_website"=>"Website"
											);
?>