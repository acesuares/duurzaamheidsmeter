<?

// core data for standard letter

// init	some var
	include("init.inc");

	include("get_gemeente.inc");
	include("get_vragen.inc");  
	include("get_g2v.inc");
	include("get_correctie.inc");
	include("get_indicators.inc");
	
	// the header row.
	$html_out = "gemeente~provincie~inwoners~";
	foreach ( $indicators as $indicators_id => $indicators_naam ) {
		$html_out .= "CF $indicators_naam~";
//		$html_out .= "CR $indicators_naam~";
	}
	foreach ( $vragen as $vraag_id => $vraag ) {
		$html_out .= "$vraag->name~";
	}
	$html_out .= "\n";

	// vragen attributes
	$vraag_attributes = array( 	"id", "score", 
															"aantal_ja_2002", "aantal_ja_2003", 
															"questionfromyear", 
															"reference_question_2003", "reference_question_2005", 
															"indicator" );
	
	foreach ( $vraag_attributes as $attribute ){
		$html_out .= "$attribute~~~";
		foreach ( $indicators as $indicators_id => $indicators_naam ) {
			$html_out .= "~";
		}
		foreach ( $vragen as $vraag_id => $vraag ) {
			$html_out .= $vraag->$attribute . "~";
		}
		$html_out .= "\n";
	}
	
	
	// content	
	foreach ( $gemeente as $gemeente_id => $gemeente_naam ) {
		$html_out .= $gemeente_naam . "~";
		$html_out .= $provincie[$gemeente_id] . "~";
		$html_out .= $inwoneraantal[$gemeente_id] . "~";
		foreach ( $indicators as $indicators_id => $indicators_naam ) {
			$html_out .= $correctie_factor[$gemeente_id][$indicators_id] . "~";
//			$html_out .= $correctie_reden[$gemeente_id][$indicators_id] . "~";
		}
		foreach ( $vragen as $vraag_id => $vraag ) {
			if ( is_array( $G2V[$gemeente_id] ) && in_array( $vraag_id, $G2V[$gemeente_id] ) ):
				$html_out .= $vraag->score . "~";
			else:
				$html_out .= "0~";
			endif;
		}
		$html_out .= "\n";
	}

	header("Content-Type: application/octet-stream");
 	#header("Content-Length: ".strlen($html_out));
 	header("Content-Disposition: attachment; filename=opgemeente.csv");
	header("Cache-control: private");

/*
	echo "<PRE>";
*/

	echo $html_out;

?>