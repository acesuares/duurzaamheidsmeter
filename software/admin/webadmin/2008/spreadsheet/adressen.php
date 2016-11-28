<?php

	include("init.inc");
	$seperator = "~";
	$line_seperator="\n";
	$html_out="";
	
// velden die we willen laten zien
	$algemene_gegevens = array( "id", "name", "adres",
										"postcode", "plaats", "provincie",
										"tel", "fax", 
										"email", "website",
										"inwoneraantal"
									);
	$indicator_gegevens = array( "aanspreektitel", "voornaam", "achternaam",
										"gemeenteadres", "postcode", "plaats",
										"tel", "email",
										"datafromyear"
									);
									
	

// get gemeentes
	$gemeentes = array();

	$query  = "SELECT * ";
	$query .= " FROM gemeente ";
	$query .= " ORDER BY name ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$gemeentes[$row->name] = $row;
	endwhile;

// get provincies	

	$provincies = array();

	$query  = "SELECT * ";
	$query .= " FROM provincie ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$provincies[$row->id] = $row->name;
	endwhile;
	
// get aanspreektitels

	$aanspreektitels = array();

	$query  = "SELECT * ";
	$query .= " FROM aanspreektitel ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$aanspreektitels[$row->id] = $row->name;
	endwhile;


// get datafromyear

	$datafromyears = array();

	$query  = "SELECT * ";
	$query .= " FROM datafromyear ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$datafromyears[$row->id] = $row->name;
	endwhile;

// get indicators
	$indicators = array( 	"1" => "kl", 
				"3" => "so",
				"4" => "rg",
				"9" => "di",
				"10" => "ka"
					);

// header
	foreach( $algemene_gegevens as $attribute_name ){
		$html_out .= $attribute_name;
		$html_out .= $seperator;
	}
// indicators 
	foreach( $indicators as $indicator_id => $indicator_code ){
		// display indicatorgebonden headers
			foreach( $indicator_gegevens as $attribute_name ){
				$html_out .= $indicator_code . "_" . $attribute_name;
				$html_out .= $seperator;
			}
	}
		// line seperator
			$html_out .= $line_seperator;

// display every gemeente
	foreach( $gemeentes as $name => $gemeente ){
		// substitute
			$gemeente->provincie = $provincies[ $gemeente->provincie_id ];
			$gemeente->kl_aanspreektitel = $aanspreektitels[ $gemeente->kl_aanspreektitel_id ];
			$gemeente->so_aanspreektitel = $aanspreektitels[ $gemeente->so_aanspreektitel_id ];
			$gemeente->rg_aanspreektitel = $aanspreektitels[ $gemeente->rg_aanspreektitel_id ];
			$gemeente->kl_datafromyear = $datafromyears[ $gemeente->kl_datafromyear_id ];
			$gemeente->so_datafromyear = $datafromyears[ $gemeente->so_datafromyear_id ];
			$gemeente->rg_datafromyear = $datafromyears[ $gemeente->rg_datafromyear_id ];
			
		// display algemene gemeentegegevens
			foreach( $algemene_gegevens as $attribute_name ){
				$html_out .= $gemeente->$attribute_name;
				$html_out .= $seperator;
			}
		// indicators 
			foreach( $indicators as $indicator_id => $indicator_code ){
				// display indicatorgebonden gemeentegegevens
					foreach( $indicator_gegevens as $attribute_name ){
						$tmp_attribute_name = $indicator_code . "_" . $attribute_name;
						$html_out .= $gemeente->$tmp_attribute_name;
						$html_out .= $seperator;
					}
			}
		// line seperator
			$html_out .= $line_seperator;
	}
// output as file
	// html headers
		header("Content-Type: application/octet-stream");
	 	header("Content-Length: ".strlen($html_out));
 		header("Content-Disposition: attachment; filename=verthorz.csv");
		header("Cache-control: private");
	// the content
		echo $html_out;
?>