<?

// core data for standard letter

// init	some var
	include("init.inc");

	include("get_gemeente.inc");
	include("get_vragen.inc");  
	include("get_g2v.inc");
	

	// the header row.
	$html_out = "indicator~vraag~maxscore~";
	foreach ( $gemeente as $gemeente_id => $gemeente_naam ) {
		$html_out .= "$gemeente_naam (" . $provincie[$gemeente_id] . ")~";
	}
	$html_out .= "\n";

	// content	
	foreach ( $vraag as $vraag_id => $vraag_naam ) {
		$html_out .= $indicator[$vraag_id] . "~";
		$html_out .= "$vraag_naam~";
		$html_out .= $score[$vraag_id] . "~";
		foreach ( $gemeente as $gemeente_id => $gemeente_naam ) {
			if ( is_array( $G2V[$gemeente_id] ) && in_array( $vraag_id, $G2V[$gemeente_id] ) ):
				$html_out .= $score[$vraag_id] . "~";
			else:
				$html_out .= "0~";
			endif;
		}
		$html_out .= "\n";
	}
	header("Content-Type: application/octet-stream");
 	header("Content-Length: ".strlen($html_out));
 	header("Content-Disposition: attachment; filename=opvragen.csv");
	header("Cache-control: private");

/*
	echo "<PRE>";
*/

	echo $html_out;

?>