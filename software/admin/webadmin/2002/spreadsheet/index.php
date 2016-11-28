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
									
	$indicator_gegevens = array();
	$correctie_gegevens = array( "correctie_factor", "correctie_reden"
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
/*
	$query  = "SELECT * ";
	$query .= " FROM aanspreektitel ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$aanspreektitels[$row->id] = $row->name;
	endwhile;
*/

// get datafromyear

	$datafromyears = array();
/*
	$query  = "SELECT * ";
	$query .= " FROM datafromyear ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$datafromyears[$row->id] = $row->name;
	endwhile;
*/
// get vraags

	$vraags = array();

	$query  = "SELECT * ";
	$query .= " FROM vraag ";
	$query .= " WHERE active='Y' ";
	$query .= " ORDER BY name ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$vraags[$row->id] = $row;
	endwhile;
// get indicators
	$indicators = array();

	$query  = "SELECT id,name ";
	$query .= " FROM indicator ";
	$query .= " WHERE active='Y' AND id <> 8 ";
	$query .= " ORDER BY name ";

 	$result=mysql($WebEngine_database,$query);

	while($row = mysql_fetch_object( $result ) ):
		$indicators[$row->id] = $row;
	endwhile;
// header
	foreach( $algemene_gegevens as $attribute_name ){
		$html_out .= $attribute_name;
		$html_out .= $seperator;
	}
// indicators 
	foreach( $indicators as $indicator_id => $indicator_code ){
		// display indicatorgebonden headers
		// vragen
			foreach( $vraags as $vraag_id => $vraag ){
				if ( $vraag->indicator_id == $indicator_id ):
					$html_out .= $vraag->name;
					$html_out .= $seperator;
				endif;
			}			
		// correctie headers
			foreach( $correctie_gegevens as $attribute_name ){
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
		// get beantwoorde vragen
			$answers = array();
			$query  = "SELECT * ";
			$query .= " FROM gemeente2vraag ";
			$query .= " WHERE gemeente_id = " . $gemeente->id ;

		 	$result = mysql($WebEngine_database,$query);

			if ( mysql_num_rows( $result ) > 0 ):
				while( $row = mysql_fetch_object( $result ) ) {
					$answers[ $row->vraag_id ] = $vraags[ $row->vraag_id ]->score;
					// for the reference questions
					if ( $answers[ $row->vraag_id ] == 0 ):
						$answers[ $row->vraag_id ] = -1;
					endif;
				}
			endif;
		// correcties
			$correcties = array();

			$query  = "SELECT * ";
			$query .= " FROM correctie ";
			$query .= " WHERE gemeente_id = " . $gemeente->id;

		 	$result=mysql($WebEngine_database,$query);

			while($row = mysql_fetch_object( $result ) ):
				$correcties[$row->indicator_id] = $row;
			endwhile;
			
		// display algemene gemeentegegevens
			foreach( $algemene_gegevens as $attribute_name ){
				$html_out .= $gemeente->$attribute_name;
				$html_out .= $seperator;
			}
		// indicators 
			foreach( $indicators as $indicator_id => $indicator_code ){
				// beantwoorde en niet-betantwoorde vragen
					foreach( $vraags as $vraag_id => $vraag ){
						if ( $vraag->indicator_id == $indicator_id ):
							if ( $answers[ $vraag_id ] ):
								$html_out .= $answers[ $vraag_id ];
								$html_out .= $seperator;
							else:
								$html_out .= $seperator;
							endif;
						endif;
					}			
				// correcties
					foreach( $correctie_gegevens as $attribute_name ){
						$html_out .= "\"" . $correcties[$indicator_id]->$attribute_name . "\"";
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