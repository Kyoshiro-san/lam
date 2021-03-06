<?PHP
use \LAM\PDF\PDFStructure;

/*
$Id$

  This code is part of LDAP Account Manager (http://www.ldap-account-manager.org/)
  Copyright (C) 2003 - 2006  Michael Duergner
                2007 - 2013  Roland Gruber

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
 * LDAP Account Manager PDF printing library. It consists of lamPDF class,
 * the createModulePDF() function that may be called by other pages
 * and furthermore some helper functions.
 *
 * @author Michael Duergner
 * @author Roland Gruber
 * @package PDF
 */

/**
 * Creates a LAM information page in PDF format.
 *
 * @package PDF
 * @author Michael Duergner
 * @author Roland Gruber
 */
class lamPDF extends UFPDF {

	/**
	 * PDF structure
	 */
	private $structure;

	/**
	* current active font name
	*/
	private $fontName;

	/**
	* list of supported fonts
	* format: <font name> => array(<file for default font style>, <file for bold>, <italic>, <bold italic>)
	*/
	private $fontList = array(
		'Dejavu' => array('DejaVuSerif.php', 'DejaVuSerif-Bold.php', 'DejaVuSerif-Bold.php', 'DejaVuSerif.php')
	);

	/**
	 * Creates a new lamPDF object.
	 *
	 * @param PDFStructure structure
	 * @param String $fontName font name
	 */
	function __construct($structure, $fontName) {
		$this->fontName = $fontName;
		if (!defined('FPDF_FONTPATH')) {
			define('FPDF_FONTPATH', dirname(__FILE__) . '/font/');
		}
		// Call constructor of superclass
		parent::__construct('P','mm','A4');

		$this->structure = $structure;

		// Open PDF file and write some basic information
		$this->Open();
		$this->AddFont($this->fontName, '', $this->fontList[$this->fontName][0]);
		$this->AddFont($this->fontName, 'B', $this->fontList[$this->fontName][1]);
		$this->AddFont($this->fontName, 'I', $this->fontList[$this->fontName][2]);
		$this->AddFont($this->fontName, 'BI', $this->fontList[$this->fontName][3]);
		$this->setFont($this->fontName,"",12);
		$this->setTitle($structure->getTitle());
		$this->setCreator("LDAP Account Manager");
		$this->setMargins('10.0','10.0','10.0');
		$this->setAutoPageBreak(true,'20.0');
	}

	/**
	 * Creates the PDF page header.
	 */
	function header() {
		if($this->structure->getLogo() != 'none') {
			$imageFile = substr(__FILE__,0,strlen(__FILE__)- 14) . "config/pdf/" . $_SESSION['config']->getName() . "/logos/" . $this->structure->getLogo();
			$imgProperties = getimagesize($imageFile);
			$this->Image($imageFile,10,15,($imgProperties[0] / $this->k) / 5, ($imgProperties[1] / $this->k) / 5,0,"JPG");
		}
		$this->SetY(23);
		$this->SetFont($this->fontName,"B",18);
		$this->Cell(170,5,$this->structure->getTitle(), 0, 1, "R", 0);
		$this->Ln(3);
		$this->SetLineWidth(0.4);
		$this->Line(10,38,200,38);
		$this->Line(10,40,200,40);
		$this->SetY(50);
		//set folding marks
		if ($this->structure->getFoldingMarks() == PDFStructure::FOLDING_STANDARD) {
			$this->SetLineWidth(0.2);
			$foldingMarks = array(97, 202);
			foreach ($foldingMarks as $mark) {
				$this->Line(0, $mark, 5, $mark);
			}
		}
	}

	/**
	 * Creates the PDF page footer.
	 */
	function footer() {
		$this->SetLineWidth(0.4);
		$this->Line(10,280,200,280);
		$this->Line(10,282,200,282);
		$this->SetY(286);
		$this->SetFont($this->fontName,"",7);
		$this->Cell(0,5,_("This document was automatically created by LDAP Account Manager") . ' (' . getFormattedTime('Y-m-d H:i:s T') . ')',0,0,"C",0);
	}
}

?>