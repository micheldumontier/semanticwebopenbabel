#include "ClassCondition.h"
#include <strings.h>

namespace Owl
{
	//--
	ClassCondition::ClassCondition() :
	m_sOn(""), m_sValue(""), m_sOperation("")
	{
	}

	

	//--
	ClassCondition::ClassCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation) :
	m_sOn(refOn), m_sValue(refValue), m_sOperation(refOperation)
	{
	}


	//--
	const std::string& ClassCondition::GetOn() const
	{
		return(m_sOn);
	}



	//--
	const std::string& ClassCondition::GetValue() const
	{
		return(m_sValue);
	}



	//--
	const std::string& ClassCondition::GetOperation() const
	{
		return(m_sOperation);
	}



	//--
	const std::string& ClassCondition::VGetDescription() const
	{
		static std::string sDescription = "Condition";
		return(sDescription);
	}


	
	//--
	// [rad] condition is a leaf, since it cannot have children (unlike unions and intersections)
	bool ClassCondition::VIsLeaf()
	{
		return(true);
	}




	//--
	// [rad] used to make look like descriptors are part of the molecule
	bool ClassCondition::ProcessDescriptor(OpenBabel::OBMol* pMolecule)
	{
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;

		std::istringstream ssConv;
		int iValue;
		double dValue;		
		

		for(iter_data = pMolecule->BeginData(); iter_data != pMolecule->EndData(); iter_data++)
		{
			// [rad] Take care of PairData only for now
			if(1 == (*iter_data)->GetDataType())
			{
				pAuxData = (OpenBabel::OBPairData*)(*iter_data);
				sQualityName = pAuxData->GetAttribute();
				sQualityValue = pAuxData->GetValue();

				if(!sQualityName.compare("PUBCHEM_COMPOUND_CID") && !m_sOn.compare("MOLECULE_PUBCHEM_COMPOUND_CID")) 
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_HBOND_ACCEPTOR"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_DONOR") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_HBOND_DONOR"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_ROTATABLE_BOND") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_ROTATABLE_BOND"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_SUBSKEYS") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_SUBSKEYS"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_OPENEYE_NAME") && !m_sOn.compare("MOLECULE_PUBCHEM_IUPAC_OPENEYE_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_CAS_NAME") && !m_sOn.compare("MOLECULE_PUBCHEM_IUPAC_CAS_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_NAME") && !m_sOn.compare("MOLECULE_PUBCHEM_IUPAC_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME") && !m_sOn.compare("MOLECULE_PUBCHEM_IUPAC_SYSTEMATIC_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME") && !m_sOn.compare("MOLECULE_PUBCHEM_IUPAC_TRADITIONAL_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_NIST_INCHI") && !m_sOn.compare("MOLECULE_PUBCHEM_NIST_INCHI"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_XLOGP") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_XLOGP"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_EXACT_MASS") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_EXACT_MASS"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MF") && !m_sOn.compare("MOLECULE_PUBCHEM_OPENEYE_MF"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MW") && !m_sOn.compare("MOLECULE_PUBCHEM_OPENEYE_MW"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_CAN_SMILES") && !m_sOn.compare("MOLECULE_PUBCHEM_OPENEYE_CAN_SMILES"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_ISO_SMILES") && !m_sOn.compare("MOLECULE_PUBCHEM_OPENEYE_ISO_SMILES"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TPSA") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_TPSA"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT") && !m_sOn.compare("MOLECULE_PUBCHEM_OPENEYE_MONOISOTOPICWT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_TOTAL_CHARGE") && !m_sOn.compare("MOLECULE_PUBCHEM_TOTAL_CHARGE"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_HEAVY_ATOM_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_HEAVY_ATOM_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_ATOM_DEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_ATOM_UDEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_DEF_STEREO_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_BOND_DEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_BOND_UDEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_ISOTOPIC_ATOM_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_COMPONENT_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_COMPONENT_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TAUTO_COUNT") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_TAUTO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BONDANNOTATIONS") && !m_sOn.compare("MOLECULE_PUBCHEM_BONDANNOTATIONS"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_COMPOUND_CANONICALIZED") && !m_sOn.compare("MOLECULE_PUBCHEM_COMPOUND_CANONICALIZED"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_COMPLEXITY") && !m_sOn.compare("MOLECULE_PUBCHEM_CACTVS_COMPLEXITY"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_FORMULA") && !m_sOn.compare("MOLECULE_PUBCHEM_MOLECULAR_FORMULA"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_WEIGHT") && !m_sOn.compare("MOLECULE_PUBCHEM_MOLECULAR_WEIGHT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_EXACT_MASS") && !m_sOn.compare("MOLECULE_PUBCHEM_EXACT_MASS"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MONOISOTOPIC_WEIGHT") && !m_sOn.compare("MOLECULE_PUBCHEM_MONOISOTOPIC_WEIGHT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
			}

		}

		return(false);
	}



	//--
	// [rad] We need to satisfy this condition
	bool ClassCondition::VSatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux)
	{
		// [rad] on must be "VALUE", this is a special case for descriptors
		if(m_sOn.compare("VALUE")) return(false);
		
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;

		std::istringstream ssConv;
		int iValue;
		double dValue;		
		

		for(iter_data = pMolecule->BeginData(); iter_data != pMolecule->EndData(); iter_data++)
		{
			// [rad] Take care of PairData only for now
			if(1 == (*iter_data)->GetDataType())
			{
				pAuxData = (OpenBabel::OBPairData*)(*iter_data);
				sQualityName = pAuxData->GetAttribute();
				sQualityValue = pAuxData->GetValue();

				if(!sQualityName.compare("PUBCHEM_COMPOUND_CID") && !refAux.compare("PUBCHEM_COMPOUND_CID")) 
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR") && !refAux.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_DONOR") && !refAux.compare("PUBCHEM_CACTVS_HBOND_DONOR"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_ROTATABLE_BOND") && !refAux.compare("PUBCHEM_CACTVS_ROTATABLE_BOND"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_SUBSKEYS") && !refAux.compare("PUBCHEM_CACTVS_SUBSKEYS"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_OPENEYE_NAME") && !refAux.compare("PUBCHEM_IUPAC_OPENEYE_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_CAS_NAME") && !refAux.compare("PUBCHEM_IUPAC_CAS_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_NAME") && !refAux.compare("PUBCHEM_IUPAC_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME") && !refAux.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME") && !refAux.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_NIST_INCHI") && !refAux.compare("PUBCHEM_NIST_INCHI"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_XLOGP") && !refAux.compare("PUBCHEM_CACTVS_XLOGP"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_EXACT_MASS") && !refAux.compare("PUBCHEM_CACTVS_EXACT_MASS"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MF") && !refAux.compare("PUBCHEM_OPENEYE_MF"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MW") && !refAux.compare("PUBCHEM_OPENEYE_MW"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_CAN_SMILES") && !refAux.compare("PUBCHEM_OPENEYE_CAN_SMILES"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_ISO_SMILES") && !refAux.compare("PUBCHEM_OPENEYE_ISO_SMILES"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TPSA") && !refAux.compare("PUBCHEM_CACTVS_TPSA"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT") && !refAux.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_TOTAL_CHARGE") && !refAux.compare("PUBCHEM_TOTAL_CHARGE"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_HEAVY_ATOM_COUNT") && !refAux.compare("PUBCHEM_HEAVY_ATOM_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_DEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_BOND_DEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT") && !refAux.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_COMPONENT_COUNT") && !refAux.compare("PUBCHEM_COMPONENT_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TAUTO_COUNT") && !refAux.compare("PUBCHEM_CACTVS_TAUTO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BONDANNOTATIONS") && !refAux.compare("PUBCHEM_BONDANNOTATIONS"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_COMPOUND_CANONICALIZED") && !refAux.compare("PUBCHEM_COMPOUND_CANONICALIZED"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_COMPLEXITY") && !refAux.compare("PUBCHEM_CACTVS_COMPLEXITY"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_FORMULA") && !refAux.compare("PUBCHEM_MOLECULAR_FORMULA"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_WEIGHT") && !refAux.compare("PUBCHEM_MOLECULAR_WEIGHT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_EXACT_MASS") && !refAux.compare("PUBCHEM_EXACT_MASS"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MONOISOTOPIC_WEIGHT") && !refAux.compare("PUBCHEM_MONOISOTOPIC_WEIGHT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
			}

		}

		return(false);	
	}
	
	
	
		
	//--
	// [rad] we need to satisfy this specific condition
	bool ClassCondition::VSatisfy(OpenBabel::OBMol* pMolecule)
	{
		if(!m_sOn.compare("MOLECULE_ATOM_COUNT"))
		{
			return(ProcessInt(pMolecule->NumAtoms()));
		}
		else if(!m_sOn.compare("MOLECULE_BOND_COUNT"))
		{
			return(ProcessInt(pMolecule->NumBonds()));
		}
		else if(!m_sOn.compare("MOLECULE_RING_COUNT"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pMolecule->GetSSSR();
			return(ProcessInt(refSSSR.size()));
		}
		else if(!m_sOn.compare("MOLECULE_HEAVY_HYDROGEN_COUNT"))
		{
			return(ProcessInt(pMolecule->NumHvyAtoms()));
		}
		else if(!m_sOn.compare("MOLECULE_RESIDUE_COUNT"))
		{
			return(ProcessInt(pMolecule->NumResidues()));
		}
		else if(!m_sOn.compare("MOLECULE_ROTOR_COUNT"))
		{
			return(ProcessInt(pMolecule->NumRotors()));
		}
		else if(!m_sOn.compare("MOLECULE_FORMULA"))
		{
			return(ProcessString(pMolecule->GetFormula()));
		}
		else if(!m_sOn.compare("MOLECULE_FORMATION_HEAT"))
		{
			return(ProcessDouble(pMolecule->GetEnergy()));
		}
		else if(!m_sOn.compare("MOLECULE_STANDARD_MOLAR_MASS"))
		{
			return(ProcessDouble(pMolecule->GetMolWt()));
		}
		else if(!m_sOn.compare("MOLECULE_EXACT_MASS"))
		{
			return(ProcessDouble(pMolecule->GetExactMass()));
		}
		else if(!m_sOn.compare("MOLECULE_TOTAL_CHARGE"))
		{
			return(ProcessInt(pMolecule->GetTotalCharge()));
		}
		else if(!m_sOn.compare("MOLECULE_SPIN_MULTIPLICITY"))
		{
			return(ProcessInt(pMolecule->GetTotalSpinMultiplicity()));
		}
		else if(!m_sOn.compare("MOLECULE_IS_CHIRAL"))
		{
			return(ProcessBool(pMolecule->IsChiral()));
		}
		else if(!m_sOn.compare("MOLECULE_HAS_AROMATIC_RING"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pMolecule->GetSSSR();
			bool bAromatic = false;
			std::vector<OpenBabel::OBRing*>::iterator iter_rings = refSSSR.begin();
			while(iter_rings != refSSSR.end())
			{
				if((*iter_rings)->IsAromatic())
				{
					bAromatic = true;
					break;
				}

				iter_rings++;
			}

			return(ProcessBool(bAromatic));
		}
		else if(!m_sOn.compare("MOLECULE_HAS_HOMOCYCLIC_RING"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pMolecule->GetSSSR();
			std::vector<OpenBabel::OBRing*>::iterator iter_rings = refSSSR.begin();
			OpenBabel::OBAtom* pAtom;

			bool bHomoCyclic = true;

			while(iter_rings != refSSSR.end())
			{
				std::vector<int>::iterator iter_path = (*iter_rings)->_path.begin();
				while(iter_path != (*iter_rings)->_path.end())
				{
					pAtom = pMolecule->GetAtom((*iter_path));

					if(pAtom)
					{
						if(6 != pAtom->GetAtomicNum())
						{
							bHomoCyclic = false;
							break;
						}
					}

					iter_path++;
				}

				if(!bHomoCyclic) break;

				iter_rings++;
			}
		
			return(ProcessBool(bHomoCyclic));
		}
		else if(!m_sOn.compare("MOLECULE_HAS_HETEROCYCLIC_RING"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pMolecule->GetSSSR();
			std::vector<OpenBabel::OBRing*>::iterator iter_rings = refSSSR.begin();
			OpenBabel::OBAtom* pAtom;

			bool bHeteroCyclic = false;

			while(iter_rings != refSSSR.end())
			{
				std::vector<int>::iterator iter_path = (*iter_rings)->_path.begin();
				while(iter_path != (*iter_rings)->_path.end())
				{
					pAtom = pMolecule->GetAtom((*iter_path));

					if(pAtom)
					{
						if(6 != pAtom->GetAtomicNum())
						{
							bHeteroCyclic = true;
							break;
						}
					}

					iter_path++;
				}

				if(bHeteroCyclic) break;

				iter_rings++;
			}
		
			return(ProcessBool(bHeteroCyclic));
		}
		else
		{
			// [rad] check if this is a descriptor
			if(ProcessDescriptor(pMolecule))
			{
				return(true);
			}
		}
		

		return(false);
	}





	//--
	// [rad] we need to satisfy this specific condition
	bool ClassCondition::VSatisfy(OpenBabel::OBAtom* pAtom)
	{
		if(!m_sOn.compare("ATOM_ATOMIC_NUMBER"))
		{
			return(ProcessInt(pAtom->GetAtomicNum()));
		}
		else if(!m_sOn.compare("ATOM_FORMAL_CHARGE"))
		{
			return(ProcessInt(pAtom->GetFormalCharge()));
		}
		else if(!m_sOn.compare("ATOM_ISOTOPE"))
		{
			return(ProcessInt(pAtom->GetIsotope()));
		}
		else if(!m_sOn.compare("ATOM_SPIN_MULTIPLICITY"))
		{
			return(ProcessInt(pAtom->GetSpinMultiplicity()));
		}
		else if(!m_sOn.compare("ATOM_ATOMIC_MASS"))
		{
			return(ProcessDouble(pAtom->GetAtomicMass()));
		}
		else if(!m_sOn.compare("ATOM_EXACT_MASS"))
		{
			return(ProcessDouble(pAtom->GetExactMass()));
		}
		else if(!m_sOn.compare("ATOM_INTERNAL_INDEX"))
		{
			return(ProcessInt(pAtom->GetIdx()));
		}
		else if(!m_sOn.compare("ATOM_VALENCE"))
		{
			return(ProcessInt(pAtom->GetValence()));
		}
		else if(!m_sOn.compare("ATOM_HYBRIDIZATION"))
		{
			return(ProcessInt(pAtom->GetHyb()));
		}
		else if(!m_sOn.compare("ATOM_IMPLICIT_VALENCE"))
		{
			return(ProcessInt(pAtom->GetImplicitValence()));
		}
		else if(!m_sOn.compare("ATOM_HEAVY_VALENCE"))
		{
			return(ProcessInt(pAtom->GetHvyValence()));
		}
		else if(!m_sOn.compare("ATOM_HETERO_VALENCE"))
		{
			return(ProcessInt(pAtom->GetHeteroValence()));
		}
		else if(!m_sOn.compare("ATOM_COORDINATE_X"))
		{
			return(ProcessDouble(pAtom->GetX()));
		}
		else if(!m_sOn.compare("ATOM_COORDINATE_Y"))
		{
			return(ProcessDouble(pAtom->GetY()));
		}
		else if(!m_sOn.compare("ATOM_COORDINATE_Z"))
		{
			return(ProcessDouble(pAtom->GetZ()));
		}
		else if(!m_sOn.compare("ATOM_PARTIAL_CHARGE"))
		{
			return(ProcessDouble(pAtom->GetPartialCharge()));
		}
		else if(!m_sOn.compare("ATOM_FREE_OXYGEN_COUNT"))
		{
			return(ProcessInt(pAtom->CountFreeOxygens()));
		}
		else if(!m_sOn.compare("ATOM_IMPLICIT_HYDROGEN_COUNT"))
		{
			return(ProcessInt(pAtom->ImplicitHydrogenCount()));
		}
		else if(!m_sOn.compare("ATOM_PARTICIPANT_RING_COUNT"))
		{
			return(ProcessInt(pAtom->MemberOfRingCount()));
		}
		else if(!m_sOn.compare("ATOM_AVERAGE_BOND_ANGLE"))
		{
			return(ProcessDouble(pAtom->AverageBondAngle()));
		}
		else if(!m_sOn.compare("ATOM_SMALLEST_BOND_ANGLE"))
		{
			return(ProcessDouble(pAtom->SmallestBondAngle()));
		}
		else if(!m_sOn.compare("ATOM_IS_IN_RING"))
		{
			return(ProcessBool(pAtom->IsInRing()));
		}
		else if(!m_sOn.compare("ATOM_IS_HETERO_ATOM"))
		{
			return(ProcessBool(pAtom->IsHeteroatom()));
		}
		else if(!m_sOn.compare("ATOM_IS_AROMATIC"))
		{
			return(ProcessBool(pAtom->IsAromatic()));
		}
		else if(!m_sOn.compare("ATOM_IS_CHIRAL"))
		{
			return(ProcessBool(pAtom->IsChiral()));
		}
		else if(!m_sOn.compare("ATOM_HAS_SINGLE_BOND"))
		{
			return(ProcessBool(pAtom->HasSingleBond()));
		}
		else if(!m_sOn.compare("ATOM_HAS_DOUBLE_BOND"))
		{
			return(ProcessBool(pAtom->HasDoubleBond()));
		}
		else if(!m_sOn.compare("ATOM_HAS_TRIPLE_BOND"))
		{
			return(ProcessBool(pAtom->HasBondOfOrder(3)));
		}		
		else if(!m_sOn.compare("ATOM_HAS_AROMATIC_BOND"))
		{
			return(ProcessBool(pAtom->HasAromaticBond()));
		}		
		else if(!m_sOn.compare("ATOM_SINGLE_BOND_COUNT"))
		{
			return(ProcessInt(pAtom->CountBondsOfOrder(1)));
		}
		else if(!m_sOn.compare("ATOM_DOUBLE_BOND_COUNT"))
		{
			return(ProcessInt(pAtom->CountBondsOfOrder(2)));
		}
		else if(!m_sOn.compare("ATOM_TRIPLE_BOND_COUNT"))
		{
			return(ProcessInt(pAtom->CountBondsOfOrder(3)));
		}
		else if(!m_sOn.compare("ATOM_AROMATIC_BOND_COUNT"))
		{
			return(ProcessInt(pAtom->CountBondsOfOrder(5)));
		}
		else if(!m_sOn.compare("ATOM_BOND_COUNT"))
		{
			//return(ProcessInt(pAtom->CountBondsOfOrder(5)));
			OpenBabel::OBBond* pBond;
			std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bond;
			int iCount = 0;

			for(pBond = pAtom->BeginBond(iter_bond); pBond; pBond = pAtom->NextBond(iter_bond))
			{
				iCount++;
			}
	
			return(ProcessInt(iCount));
		}

	
		return(false);
	}





	//--
	// [rad] we need to satisfy this specific condition
	bool ClassCondition::VSatisfy(OpenBabel::OBBond* pBond)
	{
		if(!m_sOn.compare("BOND_ORDER"))
		{
			return(ProcessInt(pBond->GetBondOrder()));
		}
		else if(!m_sOn.compare("BOND_EQUILIBRIUM_LENGTH"))
		{
			return(ProcessDouble(pBond->GetEquibLength()));
		}
		else if(!m_sOn.compare("BOND_LENGTH"))
		{
			return(ProcessDouble(pBond->GetLength()));
		}
		else if(!m_sOn.compare("BOND_IS_AROMATIC"))
		{
			return(ProcessBool(pBond->IsAromatic()));
		}
		else if(!m_sOn.compare("BOND_IS_IN_RING"))
		{
			return(ProcessBool(pBond->IsInRing()));
		}
		else if(!m_sOn.compare("BOND_IS_ROTOR"))
		{
			return(ProcessBool(pBond->IsRotor()));
		}
		else if(!m_sOn.compare("BOND_IS_AMIDE"))
		{
			return(ProcessBool(pBond->IsAmide()));
		}
		else if(!m_sOn.compare("BOND_IS_PRIMARY_AMIDE"))
		{
			return(ProcessBool(pBond->IsPrimaryAmide()));
		}
		else if(!m_sOn.compare("BOND_IS_SECONDARY_AMIDE"))
		{
			return(ProcessBool(pBond->IsSecondaryAmide()));
		}
		else if(!m_sOn.compare("BOND_IS_ESTER"))
		{
			return(ProcessBool(pBond->IsEster()));
		}
		else if(!m_sOn.compare("BOND_IS_CARBONYL"))
		{
			return(ProcessBool(pBond->IsCarbonyl()));
		}
		else if(!m_sOn.compare("BOND_IS_SINGLE_BOND"))
		{
			return(ProcessBool(pBond->IsSingle()));
		}
		else if(!m_sOn.compare("BOND_IS_DOUBLE_BOND"))
		{
			return(ProcessBool(pBond->IsDouble()));
		}
		else if(!m_sOn.compare("BOND_IS_TRIPLE_BOND"))
		{
			return(ProcessBool(pBond->IsTriple()));
		}
		else if(!m_sOn.compare("BOND_IS_CLOSURE_BOND"))
		{
			return(ProcessBool(pBond->IsClosure()));
		}
		else if(!m_sOn.compare("BOND_IS_UP"))
		{
			return(ProcessBool(pBond->IsUp()));
		}
		else if(!m_sOn.compare("BOND_IS_DOWN"))
		{
			return(ProcessBool(pBond->IsDown()));
		}
		else if(!m_sOn.compare("BOND_IS_HASH"))
		{
			return(ProcessBool(pBond->IsHash()));
		}
		else if(!m_sOn.compare("BOND_IS_WEDGE"))
		{
			return(ProcessBool(pBond->IsWedge()));
		}

		return(false);
	}




	//--
	// [rad] we need to satisfy this specific condition
	bool ClassCondition::VSatisfy(OpenBabel::OBRing* pRing)
	{
		if(!m_sOn.compare("RING_SIZE"))
		{
			return(ProcessInt(pRing->Size()));
		}
		else if(!m_sOn.compare("RING_IS_AROMATIC"))
		{
			return(ProcessBool(pRing->IsAromatic()));
		}
		else if(!m_sOn.compare("RING_IS_HOMOCYCLIC"))
		{
			OpenBabel::OBMol* pMolecule = pRing->GetParent();
			OpenBabel::OBAtom* pAtom;

			bool bHomoCyclic = true;

			std::vector<int>::iterator iter_path = pRing->_path.begin();
			while(iter_path != pRing->_path.end())
			{
				pAtom = pMolecule->GetAtom((*iter_path));

				if(pAtom)
				{
					if(6 != pAtom->GetAtomicNum())
					{
						bHomoCyclic = false;
						break;
					}
				}

				iter_path++;
			}

			return(ProcessBool(bHomoCyclic));
		}
		else if(!m_sOn.compare("RING_IS_HETEROCYCLIC"))
		{
			OpenBabel::OBMol* pMolecule = pRing->GetParent();
			OpenBabel::OBAtom* pAtom;

			bool bHeteroCyclic = false;

			std::vector<int>::iterator iter_path = pRing->_path.begin();
			while(iter_path != pRing->_path.end())
			{
				pAtom = pMolecule->GetAtom((*iter_path));

				if(pAtom)
				{
					if(6 != pAtom->GetAtomicNum())
					{
						bHeteroCyclic = true;
						break;
					}
				}

				iter_path++;
			}

			return(ProcessBool(bHeteroCyclic));
		}

		return(false);
	}



	//--
	bool ClassCondition::ProcessInt(int iValue)
	{
		int iLeftValue = 0;	
		std::istringstream ssConv;
		ssConv.str(m_sValue);
		ssConv >> iLeftValue;

		if(!m_sOperation.compare("equal"))
		{
			return(iLeftValue == iValue);
		}
		else if(!m_sOperation.compare("less"))
		{
			return(iLeftValue > iValue);
		}
		else if(!m_sOperation.compare("greater"))
		{
			return(iLeftValue < iValue);
		}
		else if(!m_sOperation.compare("not equal"))
		{
			return(iLeftValue != iValue);
		}

		return(false);
	}



	//--
	bool ClassCondition::ProcessBool(bool bValue)
	{
		bool bLeftValue = false;

		//if(!_strcmpi(m_sValue.c_str(), "true") || !_strcmpi(m_sValue.c_str(), "1") || !_strcmpi(m_sValue.c_str(), "yes"))
		if(!strcasecmp(m_sValue.c_str(), "true") || !strcasecmp(m_sValue.c_str(), "1") || !strcasecmp(m_sValue.c_str(), "yes"))
		{
			bLeftValue = true;
		}


		if(!m_sOperation.compare("equal"))
		{
			return(bLeftValue == bValue);
		}
		else if(!m_sOperation.compare("not equal"))
		{
			return(bLeftValue != bValue);
		}

		return(false);		
	}


	//--
	bool ClassCondition::ProcessString(const std::string& sValue)
	{
		if(!m_sOperation.compare("equal"))
		{
			return(!m_sValue.compare(sValue));
		}
		else if(!m_sOperation.compare("not equal"))
		{
			return(!m_sValue.compare(sValue));
		}

		return(false);		
	}




	//--
	bool ClassCondition::ProcessDouble(double dValue)
	{
		double dLeftValue = 0;	
		std::istringstream ssConv;
		ssConv.str(m_sValue);
		ssConv >> dLeftValue;

		if(!m_sOperation.compare("equal"))
		{
			return(dLeftValue == dValue);
		}
		else if(!m_sOperation.compare("less"))
		{
			return(dLeftValue > dValue);
		}
		else if(!m_sOperation.compare("greater"))
		{
			return(dLeftValue < dValue);
		}
		else if(!m_sOperation.compare("not equal"))
		{
			return(dLeftValue != dValue);
		}

		return(false);
	}


}