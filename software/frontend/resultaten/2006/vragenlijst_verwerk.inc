<?php
// get out of here, it's not 2007 anymore
	header( "HTTP/1.0 404 Not Found" );
	header( "Location: {$settings->site_url}/$default_year" );
	exit;
/*
	// sanitize gemeente_id
		$gemeente_id = abs( intval ( $_POST[ "gemeente_id" ] ) );
		if ( $gemeente_id != $_POST[ "gemeente_id" ] ):
			$gemeente_id = 0;
		endif;
		if ( ! in_array( $gemeente_id, array_keys( $gemeente_ids ) ) ):
			$gemeente_id = 0;
		endif;
	// correctie
		$scratchy = "";
		if ( $_POST[ "correctiefactor" ] != 0 && ! $_POST[ "correctiereden" ] ):
			$scratchy = "correctiereden niet ingevuld";
		endif;
	// gemeente_id, invuller_naam and invuller_tel are required		
		if ( 	$gemeente_id == 0 
				|| ! $_POST[ "invuller_naam" ] 
				|| ! $_POST[ "invuller_tel" ]
				|| $scratchy 
			):
			$out .= "<DIV ID=\"invulfout\">";
			$out .= "<H1>Er is iets fout gegaan bij het invullen.</H1>";
			$out .= "<UL>";
			if ( $gemeente_id == 0 ):
				$out .= "<LI>U hebt geen gemeente geselecteerd.";
			endif;
			if ( ! $_POST[ "invuller_naam" ] ):
				$out .= "<LI>U hebt uw naam niet ingevuld.";
			endif;
			if ( ! $_POST[ "invuller_tel" ] ):
				$out .= "<LI>U hebt uw telefoonnummer niet ingevuld.";
			endif;
			if ( $scratchy ):
				$out .= "<LI>U hebt een correctie factor ingevoerd maar geen reden opgegeven.";
			endif;
			$out .= "</UL>";
			$out .= "<H2>Gebruik de 'back' of 'terug' knop van uw browser om terug te keren.</H2>";
			$out .= "</DIV>";
		else:
			$totaal_score = 0;
			$message_body_vragen = "";
		
			$out .= "<DIV ID=\"vragenlijst\">\n";
			$out .= "<TABLE>\n";
			$out .= "<TR>\n";
			$out .= "<TD ID=\"vl-header-vraagnummer\">" . $strings[ "nr." ] . "</strong></TD>\n";
			$out .= "<TD ID=\"vl-header-vraag\">" . $strings[ "vraag" ] . "</TD>\n";
			$out .= "<TD ID=\"vl-header-jaofnee\">" . $strings[ "antwoord" ] . "</TD>\n";
			$out .= "<TD ID=\"vl-header-score\">" . $strings[ "score" ] . "</TD>\n";
			$out .= "<TD>&nbsp;</TD>\n";
			$out .= "</TR>\n";
			foreach ( $vragen as $vraagnummer => $vraag ){
				$message_body_vragen .= "$vraagnummer. ";
				$out .= "<TR>\n";
				$out .= "<TD ID=\"vl-vraagnummer\">$vraagnummer.</TD>\n";
				$out .= "<TD ID=\"vl-vraag\">";
				$out .= $vraag->title;
				$out .= "</TD>\n";
				if ( $_POST[ "vraag" ][ $vraag->id ] == "Y" ):
					$out .= "<TD ID=\"vl-jaofnee\">";
					$out .= "ja";
					$out .= "</TD>\n";
					$out .= "<TD ID=\"vl-score\">";
					$out .= $vraag->score;
					$out .= "</TD>\n";
					$totaal_score += $vraag->score;
					$update_sql .= "{$vraag->id},Y;";
					$message_body_vragen .= "ja   ";
				else:
					$out .= "<TD ID=\"vl-jaofnee\">";
					$out .= "nee";
					$out .= "</TD>\n";
					$out .= "<TD ID=\"vl-score\">";
					$out .=  "&nbsp;";
					$out .= "</TD>\n";
					$update_sql .= "{$vraag->id},N;";
					$message_body_vragen .= "nee  ";
				endif;
				$out .= "<TD>&nbsp;</TD>\n";
				$out .= "</TR>\n";
				$message_body_vragen .= stripslashes( substr( $vraag->title, 0 , 60 ) ) . "\n";		
			}
			// TOTAALSCORE 
				$out .= "<TR ID=\"vl-totaalscore-row\">\n";
				$out .= "<TD>&nbsp;</TD>\n";
				$out .= "<TD ID=\"vl-totaalscore-tekst\">Totaalscore</TD>\n";
				$out .= "<TD>&nbsp;</TD>\n";
				$out .= "<TD ID=\"vl-totaalscore-score\">$totaal_score</TD>\n";
				$out .= "<TD>&nbsp;</TD>\n";
				$out .= "</TR>\n";
				$message_body_vragen .= "\n\nTotaalscore: $totaal_score\n";
			// Correctie
				$correctiefactor = intval( $_POST[ "correctiefactor" ] );
				$correctiereden = $_POST[ "correctiereden" ];
				if ( $correctiefactor <> 0 ):
					//factor
					$out .= "<TR ID=\"vl-cfactor-row\">\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "<TD ID=\"vl-cfactor-tekst\">Correctiefactor</TD>\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "<TD ID=\"vl-cfactor-score\">$correctiefactor</TD>\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "</TR>\n";
					// gecorrigeerd
					$out .= "<TR ID=\"vl-cfactor-totaal-row\">\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "<TD ID=\"vl-cfactor-totaal-tekst\">Gecorrigeerde Totaalscore</TD>\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "<TD ID=\"vl-cfactor-totaal-score\">" . ( $totaal_score + $correctiefactor ) . "</TD>\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "</TR>\n";
					$message_body_vragen .= "Correctiefactor: $correctiefactor\n";
					$message_body_vragen .= "Gecorrigeerde Totaalscore: " . ( $totaal_score + $correctiefactor ) . "\n";
				endif;
				// gecorrigeerd in procenten
				$out .= "<TR ID=\"vl-gecorr-totaal-row\">\n";
				$out .= "<TD>&nbsp;</TD>\n";
				$out .= "<TD ID=\"vl-gecorr-totaal-tekst\"><A HREF=\"javascript:void(0)\">Totaalscore (inclusief eventuele correcties) in procenten<SPAN>&nbsp;&nbsp;In principe kan een gemeente op vrijwel alle vragen 'ja' antwoorden. 
		Voor elk positief antwoord zijn punten te behalen. De totaalscore (in 
		procenten) geeft aan hoeveel procent positief beantwoord is. Uiteraard 
		moet dit beeld gerelativeerd worden. We kunnen veel vragen niet stellen. 
		Maar het resultaat is een goede indicatie van de prestatie van de gemeente.
		</SPAN></A></TD>\n";
				$out .= "<TD>&nbsp;</TD>\n";
				$out .= "<TD ID=\"vl-gecorr-totaal-score\">";
				$out .= round ( ( $totaal_score + $correctiefactor ) / $indicators[ $onderwerp ]->max_score * 100 );
				$out .= "</TD>\n";
				$out .= "<TD ID=\"vl-gecorr-totaal-procentteken\">&nbsp;%</TD>\n";
				$out .= "</TR>\n";
				$message_body_vragen .= "Totaalscore (inclusief eventuele correcties) in procenten: ";
				$message_body_vragen .= round ( ( $totaal_score + $correctiefactor ) / $indicators[ $onderwerp ]->max_score * 100 ) . "\n\n";
				// reden
				if ( $correctiereden <> "" ):
					$out .= "<TR ID=\"vl-creden-row\">\n";
					$out .= "<TD>&nbsp;</TD>\n";
					$out .= "<TD COLSPAN=4 ID=\"vl-creden-tekst\">" . $strings[ "Reden voor correctie" ] . ": " . stripslashes( $correctiereden ) . "</TD>\n";
					$out .= "</TR>\n";
					$message_body_vragen .= $strings[ "Reden voor correctie" ] . ": " . stripslashes( $correctiereden ) . "\n\n";
				endif;
				$out .= "</TABLE>\n";
				$out .= "</DIV>\n";
			// invuller-gegevens
				$out .= "<P ID=\"gemeente-gegevens-gemeente\">\n";
				$out .= "<strong>" . $strings[ "Gemeente" ] . ": {$gemeente_ids[ $gemeente_id ]}</strong><BR>\n";
				$out .= "<BR>\n";
				$out .= "Ingevuld door: " . $_POST[ "invuller_aanspreektitel" ] . " " . stripslashes( $_POST[ "invuller_naam" ] ) . "<BR>\n";
				$out .= "Organisatie of Afdeling: " . stripslashes( $_POST[ "invuller_organisatie" ] ) . "<BR>\n";
				$out .= "Adres: " . stripslashes( $_POST[ "invuller_adres" ] ) . "<BR>\n";
				$out .= "Postcode en Plaats: " . $_POST[ "invuller_postcode" ] . " " . stripslashes( $_POST[ "invuller_plaats" ] ) . "<BR>\n";
				$out .= "Telefoon: " . $_POST[ "invuller_tel" ]  . "<BR>\n";
				$out .= "Fax: " . $_POST[ "invuller_fax" ]  . "<BR>\n";
				$out .= "E-mail: " . $_POST[ "invuller_email" ]  . "<BR>\n";
				$out .= "Website: " . $_POST[ "invuller_website" ]  . "<BR>\n";
				$out .= "<BR>\n";
				$out .= "Contactambtenaar: " . stripslashes( $_POST[ "contact_ambtenaar_naam" ] ) . "<BR>\n";
				$out .= "Contactambtenaar telefoon: " . stripslashes ( $_POST[ "contact_ambtenaar_tel" ] ) . "<BR>\n";
		// mail the results
			$message_body = "";
			$message_body .= $strings[ "Gemeente" ] . ": {$gemeente_ids[ $gemeente_id ]}\n\n";
			$message_body .= "Onderwerp: $onderwerp\n\n";
			$message_body .= "Ingevuld door: " . $_POST[ "invuller_aanspreektitel" ] . " " . stripslashes( $_POST[ "invuller_naam" ] ) . "\n";
			$message_body .= "Organisatie of Afdeling: " . stripslashes( $_POST[ "invuller_organisatie" ] ) . "\n";
			$message_body .= "Adres: " . stripslashes( $_POST[ "invuller_adres" ] ) . "\n";
			$message_body .= "Postcode en Plaats: " . $_POST[ "invuller_postcode" ] . " " . stripslashes( $_POST[ "invuller_plaats" ] ) . "\n";
			$message_body .= "Telefoon: " . $_POST[ "invuller_tel" ]  . "\n";
			$message_body .= "Fax: " . $_POST[ "invuller_fax" ]  . "\n";
			$message_body .= "E-mail: " . $_POST[ "invuller_email" ]  . "\n";
			$message_body .= "Website: " . $_POST[ "invuller_website" ]  . "\n";
			$message_body .= "\n";
			$message_body .= "Contactambtenaar: " . stripslashes( $_POST[ "contact_ambtenaar_naam" ] ) . "\n";
			$message_body .= "Contactambtenaar telefoon: " . stripslashes ( $_POST[ "contact_ambtenaar_tel" ] ) . "\n\n";
			$message_body .= "Correctiefactor: $correctiefactor\n";
			$message_body .= "Correctiereden: $correctiereden\n\n";
			$message_body .= "Databasecode:\n$update_sql\n\n\n";
			$message_body .= $message_body_vragen;
			$message_body .= "\n--einde bericht\n\n";
			$message_subject = "";
			$message_subject .= $gemeente_ids[ $gemeente_id ];
			$message_subject .= ", " . $onderwerp;
		
			$mail_to="acekiko@suares.com";
			$mail_headers="Return-Path: dzhm@duurzaamheidsmeter.nl\nFrom: dzhm@duurzaamheidsmeter.nl\nX-ASIC-application: Duurzaamheidsmeter 2006\nX-developer: Suares & Co (http://www.suares.com)";
			if ( mail( $mail_to, $message_subject, $message_body, $mail_headers ) ):
				$out .= "<P ID=\"mail-melding\">\n";
				$out .= "Uw antwoorden zijn verstuurd. Hartelijk dank voor uw bijdrage!<BR>Over enige tijd zullen de resultaten zichtbaar zijn op deze site.<BR><BR>";
				$out .= "Hierboven vindt u de gegevens die verwerkt zijn.<BR>Mocht u deze willen corrigeren, neem dan contact op via info@duurzaamheidsmeter.nl of vul de lijst opnieuw in met de vermelding: 'gecorrigeerde lijst'";
				$out .= "</P>";
			else:
				$out .= "<P ID=\"mail-melding\">\n";
				$out .= "Uw aanvraag is NIET verstuurd door technische problemen. Neem contact op met <A HREF=\"mailto:webmaster@duurzaamheidsmeter.nl\">webmaster@duurzaamheidsmeter.nl</A>. Dit zijn de niet-verstuurde gegevens: ";
				$out .= "</P>";
				$out .= "<PRE>\n";
				$out .= "$message_subject\n\n";
				$out .= "$message_body\n";
				$out .= "</PRE>\n";
			endif;
		endif;
*/
?>