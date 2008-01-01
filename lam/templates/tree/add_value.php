<?php
/*
$Id$

  This code is part of LDAP Account Manager (http://www.sourceforge.net/projects/lam)
  
  This code is based on phpLDAPadmin.
  Copyright (C) 2004  David Smith and phpLDAPadmin developers
  
  The original code was modified to fit for LDAP Account Manager by Roland Gruber.
  Copyright (C) 2005  Roland Gruber

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA


*/
 

/**
 * Adds a value to an attribute for a given dn.
 * Variables that come in as POST vars:
 *  - dn (rawurlencoded)
 *  - attr (rawurlencoded) the attribute to which we are adding a value 
 *  - new_value (form element)
 *  - binary 
 *
 * On success, redirect to the edit_dn page.
 * On failure, echo an error.
 *
 * @package lists
 * @subpackage tree
 * @author David Smith
 * @author Roland Gruber
 */

/** security functions */
include_once('../../lib/security.inc');
/** tree functions */
include_once('../../lib/tree.inc');
/** access to configuration */
include_once('../../lib/config.inc');
/** LDAP functions */
include_once('../../lib/ldap.inc');
/** status messages */
include_once('../../lib/status.inc');

// start session
startSecureSession();

// die if no write access
if (!checkIfWriteAccessIsAllowed()) die();

setlanguage();

$dn = rawurldecode( $_POST['dn'] );
$encoded_dn = rawurlencode( $dn );
$attr = $_POST['attr'];
$encoded_attr = rawurlencode( $attr );
$new_value = $_POST['new_value'];
$is_binary_val = isset( $_POST['binary'] ) ? true : false;

$ds = $_SESSION['ldap']->server();

// special case for binary attributes: 
// we must go read the data from the file.
if( $is_binary_val )
{
	$file = $_FILES['new_value']['tmp_name'];
	$f = fopen( $file, 'r' );
	$binary_value = fread( $f, filesize( $file ) );
	fclose( $f );
	$new_value = $binary_value;
}

$new_entry = array( $attr => $new_value  );

$add_result = @ldap_mod_add( $ds, $dn, $new_entry );

if( ! $add_result ) {
	echo $_SESSION['header'];
	
	echo "<title>LDAP Account Manager</title>\n";
	echo "<link rel=\"stylesheet\" type=\"text/css\" href=\"../../style/layout.css\">\n";
	echo "</head><body>\n";
	StatusMessage('ERROR', _('Adding attribute failed!'), ldap_error( $ds ));
	echo "</body></html>";
	exit;
}

header( "Location: edit.php?dn=$encoded_dn&amp;modified_attrs[]=$encoded_attr" );

?>
