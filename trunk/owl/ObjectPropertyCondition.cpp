#include "ObjectPropertyCondition.h"

#include <strings.h>

#include "RelationBond.h"
#include "RelationRing.h"
#include "RelationAtom.h"
#include "RelationMolecule.h"
#include "RelationDescriptor.h"

namespace Owl
{
	//--
	ObjectPropertyCondition::ObjectPropertyCondition() :
	m_sOn(""), m_sValue(""), m_sOperation("")
	{
	}

	

	//--
	ObjectPropertyCondition::ObjectPropertyCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation) :
	m_sOn(refOn), m_sValue(refValue), m_sOperation(refOperation)
	{
	}



	//--
	const std::string& ObjectPropertyCondition::GetOn() const
	{
		return(m_sOn);
	}



	//--
	const std::string& ObjectPropertyCondition::GetValue() const
	{
		return(m_sValue);
	}



	//--
	const std::string& ObjectPropertyCondition::GetOperation() const
	{
		return(m_sOperation);
	}



	//--
	const std::string& ObjectPropertyCondition::VGetDescription() const
	{
		static std::string sDescription = "Condition";
		return(sDescription);
	}


	
	//--
	// [rad] condition is a leaf, since it cannot have children (unlike unions and intersections)
	bool ObjectPropertyCondition::VIsLeaf()
	{
		return(true);
	}


	//--
	bool ObjectPropertyCondition::ProcessDescriptor(RelationMolecule* pRelationMolecule)
	{
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;

		std::istringstream ssConv;
		int iValue;
		double dValue;		
	
		OpenBabel::OBMol* pMolecule = pRelationMolecule->m_pMol;

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
	bool ObjectPropertyCondition::VSatisfy(RelationDescriptor* pRelDescriptor)
	{
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;

		std::istringstream ssConv;
		int iValue;
		double dValue;		
	
		OpenBabel::OBMol* pMolecule = pRelDescriptor->m_pMol;

		for(iter_data = pMolecule->BeginData(); iter_data != pMolecule->EndData(); iter_data++)
		{
			// [rad] Take care of PairData only for now
			if(1 == (*iter_data)->GetDataType())
			{
				pAuxData = (OpenBabel::OBPairData*)(*iter_data);
				sQualityName = pAuxData->GetAttribute();
				sQualityValue = pAuxData->GetValue();

				if(!sQualityName.compare("PUBCHEM_COMPOUND_CID") && !m_sOn.compare("PUBCHEM_COMPOUND_CID")) 
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR") && !m_sOn.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_DONOR") && !m_sOn.compare("PUBCHEM_CACTVS_HBOND_DONOR"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_ROTATABLE_BOND") && !m_sOn.compare("PUBCHEM_CACTVS_ROTATABLE_BOND"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_SUBSKEYS") && !m_sOn.compare("PUBCHEM_CACTVS_SUBSKEYS"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_OPENEYE_NAME") && !m_sOn.compare("PUBCHEM_IUPAC_OPENEYE_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_CAS_NAME") && !m_sOn.compare("PUBCHEM_IUPAC_CAS_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_NAME") && !m_sOn.compare("PUBCHEM_IUPAC_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME") && !m_sOn.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME") && !m_sOn.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_NIST_INCHI") && !m_sOn.compare("PUBCHEM_NIST_INCHI"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_XLOGP") && !m_sOn.compare("PUBCHEM_CACTVS_XLOGP"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_EXACT_MASS") && !m_sOn.compare("PUBCHEM_CACTVS_EXACT_MASS"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MF") && !m_sOn.compare("PUBCHEM_OPENEYE_MF"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MW") && !m_sOn.compare("PUBCHEM_OPENEYE_MW"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_CAN_SMILES") && !m_sOn.compare("PUBCHEM_OPENEYE_CAN_SMILES"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_ISO_SMILES") && !m_sOn.compare("PUBCHEM_OPENEYE_ISO_SMILES"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TPSA") && !m_sOn.compare("PUBCHEM_CACTVS_TPSA"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT") && !m_sOn.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_TOTAL_CHARGE") && !m_sOn.compare("PUBCHEM_TOTAL_CHARGE"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_HEAVY_ATOM_COUNT") && !m_sOn.compare("PUBCHEM_HEAVY_ATOM_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT") && !m_sOn.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT") && !m_sOn.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_DEF_STEREO_COUNT") && !m_sOn.compare("PUBCHEM_BOND_DEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT") && !m_sOn.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT") && !m_sOn.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_COMPONENT_COUNT") && !m_sOn.compare("PUBCHEM_COMPONENT_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TAUTO_COUNT") && !m_sOn.compare("PUBCHEM_CACTVS_TAUTO_COUNT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_BONDANNOTATIONS") && !m_sOn.compare("PUBCHEM_BONDANNOTATIONS"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_COMPOUND_CANONICALIZED") && !m_sOn.compare("PUBCHEM_COMPOUND_CANONICALIZED"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_COMPLEXITY") && !m_sOn.compare("PUBCHEM_CACTVS_COMPLEXITY"))
				{
					ssConv.str(sQualityValue);
					ssConv >> iValue;

					return(ProcessInt(iValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_FORMULA") && !m_sOn.compare("PUBCHEM_MOLECULAR_FORMULA"))
				{
					return(ProcessString(sQualityValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_WEIGHT") && !m_sOn.compare("PUBCHEM_MOLECULAR_WEIGHT"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_EXACT_MASS") && !m_sOn.compare("PUBCHEM_EXACT_MASS"))
				{
					ssConv.str(sQualityValue);
					ssConv >> dValue;

					return(ProcessDouble(dValue));
				}
				else if(!sQualityName.compare("PUBCHEM_MONOISOTOPIC_WEIGHT") && !m_sOn.compare("PUBCHEM_MONOISOTOPIC_WEIGHT"))
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
	bool ObjectPropertyCondition::VSatisfy(RelationMolecule* pRelationMolecule)
	{		
		if(!m_sOn.compare("MOLECULE_ATOM_COUNT"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->NumAtoms()));
		}
		else if(!m_sOn.compare("MOLECULE_BOND_COUNT"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->NumBonds()));
		}
		else if(!m_sOn.compare("MOLECULE_RING_COUNT"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pRelationMolecule->m_pMol->GetSSSR();
			return(ProcessInt(refSSSR.size()));
		}
		else if(!m_sOn.compare("MOLECULE_HEAVY_HYDROGEN_COUNT"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->NumHvyAtoms()));
		}
		else if(!m_sOn.compare("MOLECULE_RESIDUE_COUNT"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->NumResidues()));
		}
		else if(!m_sOn.compare("MOLECULE_ROTOR_COUNT"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->NumRotors()));
		}
		else if(!m_sOn.compare("MOLECULE_FORMULA"))
		{
			return(ProcessString(pRelationMolecule->m_pMol->GetFormula()));
		}
		else if(!m_sOn.compare("MOLECULE_FORMATION_HEAT"))
		{
			return(ProcessDouble(pRelationMolecule->m_pMol->GetEnergy()));
		}
		else if(!m_sOn.compare("MOLECULE_STANDARD_MOLAR_MASS"))
		{
			return(ProcessDouble(pRelationMolecule->m_pMol->GetMolWt()));
		}
		else if(!m_sOn.compare("MOLECULE_EXACT_MASS"))
		{
			return(ProcessDouble(pRelationMolecule->m_pMol->GetExactMass()));
		}
		else if(!m_sOn.compare("MOLECULE_TOTAL_CHARGE"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->GetTotalCharge()));
		}
		else if(!m_sOn.compare("MOLECULE_SPIN_MULTIPLICITY"))
		{
			return(ProcessInt(pRelationMolecule->m_pMol->GetTotalSpinMultiplicity()));
		}
		else if(!m_sOn.compare("MOLECULE_IS_CHIRAL"))
		{
			return(ProcessBool(pRelationMolecule->m_pMol->IsChiral()));
		}
		else if(!m_sOn.compare("MOLECULE_HAS_AROMATIC_RING"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pRelationMolecule->m_pMol->GetSSSR();
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
			std::vector<OpenBabel::OBRing*>& refSSSR = pRelationMolecule->m_pMol->GetSSSR();
			std::vector<OpenBabel::OBRing*>::iterator iter_rings = refSSSR.begin();
			OpenBabel::OBAtom* pAtom;

			bool bHomoCyclic = true;

			while(iter_rings != refSSSR.end())
			{
				std::vector<int>::iterator iter_path = (*iter_rings)->_path.begin();
				while(iter_path != (*iter_rings)->_path.end())
				{
					pAtom = pRelationMolecule->m_pMol->GetAtom((*iter_path));

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
			std::vector<OpenBabel::OBRing*>& refSSSR = pRelationMolecule->m_pMol->GetSSSR();
			std::vector<OpenBabel::OBRing*>::iterator iter_rings = refSSSR.begin();
			OpenBabel::OBAtom* pAtom;

			bool bHeteroCyclic = false;

			while(iter_rings != refSSSR.end())
			{
				std::vector<int>::iterator iter_path = (*iter_rings)->_path.begin();
				while(iter_path != (*iter_rings)->_path.end())
				{
					pAtom = pRelationMolecule->m_pMol->GetAtom((*iter_path));

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
			if(ProcessDescriptor(pRelationMolecule))
			{
				return(true);
			}
		}

		return(false);
	}


	//--
	bool ObjectPropertyCondition::VSatisfy(RelationAtom* pRelationAtom)
	{		
		if(!m_sOn.compare("ATOM_ATOMIC_NUMBER"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetAtomicNum()));
		}
		else if(!m_sOn.compare("ATOM_FORMAL_CHARGE"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetFormalCharge()));
		}
		else if(!m_sOn.compare("ATOM_ISOTOPE"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetIsotope()));
		}
		else if(!m_sOn.compare("ATOM_SPIN_MULTIPLICITY"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetSpinMultiplicity()));
		}
		else if(!m_sOn.compare("ATOM_ATOMIC_MASS"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->GetAtomicMass()));
		}
		else if(!m_sOn.compare("ATOM_EXACT_MASS"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->GetExactMass()));
		}
		else if(!m_sOn.compare("ATOM_INTERNAL_INDEX"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetIdx()));
		}
		else if(!m_sOn.compare("ATOM_VALENCE"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetValence()));
		}
		else if(!m_sOn.compare("ATOM_HYBRIDIZATION"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetHyb()));
		}
		else if(!m_sOn.compare("ATOM_IMPLICIT_VALENCE"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetImplicitValence()));
		}
		else if(!m_sOn.compare("ATOM_HEAVY_VALENCE"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetHvyValence()));
		}
		else if(!m_sOn.compare("ATOM_HETERO_VALENCE"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->GetHeteroValence()));
		}
		else if(!m_sOn.compare("ATOM_COORDINATE_X"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->GetX()));
		}
		else if(!m_sOn.compare("ATOM_COORDINATE_Y"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->GetY()));
		}
		else if(!m_sOn.compare("ATOM_COORDINATE_Z"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->GetZ()));
		}
		else if(!m_sOn.compare("ATOM_PARTIAL_CHARGE"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->GetPartialCharge()));
		}
		else if(!m_sOn.compare("ATOM_FREE_OXYGEN_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->CountFreeOxygens()));
		}
		else if(!m_sOn.compare("ATOM_IMPLICIT_HYDROGEN_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->ImplicitHydrogenCount()));
		}
		else if(!m_sOn.compare("ATOM_PARTICIPANT_RING_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->MemberOfRingCount()));
		}
		else if(!m_sOn.compare("ATOM_AVERAGE_BOND_ANGLE"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->AverageBondAngle()));
		}
		else if(!m_sOn.compare("ATOM_SMALLEST_BOND_ANGLE"))
		{
			return(ProcessDouble(pRelationAtom->m_pAtom->SmallestBondAngle()));
		}
		else if(!m_sOn.compare("ATOM_IS_IN_RING"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->IsInRing()));
		}
		else if(!m_sOn.compare("ATOM_IS_HETERO_ATOM"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->IsHeteroatom()));
		}
		else if(!m_sOn.compare("ATOM_IS_AROMATIC"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->IsAromatic()));
		}
		else if(!m_sOn.compare("ATOM_IS_CHIRAL"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->IsChiral()));
		}
		else if(!m_sOn.compare("ATOM_HAS_SINGLE_BOND"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->HasSingleBond()));
		}
		else if(!m_sOn.compare("ATOM_HAS_DOUBLE_BOND"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->HasDoubleBond()));
		}
		else if(!m_sOn.compare("ATOM_HAS_TRIPLE_BOND"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->HasBondOfOrder(3)));
		}		
		else if(!m_sOn.compare("ATOM_HAS_AROMATIC_BOND"))
		{
			return(ProcessBool(pRelationAtom->m_pAtom->HasAromaticBond()));
		}		
		else if(!m_sOn.compare("ATOM_SINGLE_BOND_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->CountBondsOfOrder(1)));
		}
		else if(!m_sOn.compare("ATOM_DOUBLE_BOND_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->CountBondsOfOrder(2)));
		}
		else if(!m_sOn.compare("ATOM_TRIPLE_BOND_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->CountBondsOfOrder(3)));
		}
		else if(!m_sOn.compare("ATOM_AROMATIC_BOND_COUNT"))
		{
			return(ProcessInt(pRelationAtom->m_pAtom->CountBondsOfOrder(5)));
		}
		else if(!m_sOn.compare("ATOM_BOND_COUNT"))
		{
			//return(ProcessInt(pAtom->CountBondsOfOrder(5)));
			OpenBabel::OBBond* pBond;
			std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bond;
			int iCount = 0;

			for(pBond = pRelationAtom->m_pAtom->BeginBond(iter_bond); pBond; pBond = pRelationAtom->m_pAtom->NextBond(iter_bond))
			{
				iCount++;
			}
	
			return(ProcessInt(iCount));
		}

	
		return(false);
	}



	//--
	bool ObjectPropertyCondition::VSatisfy(RelationBond* pRelationBond)
	{
		if(!m_sOn.compare("BOND_ORDER"))
		{
			return(ProcessInt(pRelationBond->m_pBond->GetBondOrder()));
		}
		else if(!m_sOn.compare("BOND_EQUILIBRIUM_LENGTH"))
		{
			return(ProcessDouble(pRelationBond->m_pBond->GetEquibLength()));
		}
		else if(!m_sOn.compare("BOND_LENGTH"))
		{
			return(ProcessDouble(pRelationBond->m_pBond->GetLength()));
		}
		else if(!m_sOn.compare("BOND_IS_AROMATIC"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsAromatic()));
		}
		else if(!m_sOn.compare("BOND_IS_IN_RING"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsInRing()));
		}
		else if(!m_sOn.compare("BOND_IS_ROTOR"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsRotor()));
		}
		else if(!m_sOn.compare("BOND_IS_AMIDE"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsAmide()));
		}
		else if(!m_sOn.compare("BOND_IS_PRIMARY_AMIDE"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsPrimaryAmide()));
		}
		else if(!m_sOn.compare("BOND_IS_SECONDARY_AMIDE"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsSecondaryAmide()));
		}
		else if(!m_sOn.compare("BOND_IS_ESTER"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsEster()));
		}
		else if(!m_sOn.compare("BOND_IS_CARBONYL"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsCarbonyl()));
		}
		else if(!m_sOn.compare("BOND_IS_SINGLE_BOND"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsSingle()));
		}
		else if(!m_sOn.compare("BOND_IS_DOUBLE_BOND"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsDouble()));
		}
		else if(!m_sOn.compare("BOND_IS_TRIPLE_BOND"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsTriple()));
		}
		else if(!m_sOn.compare("BOND_IS_CLOSURE_BOND"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsClosure()));
		}
		else if(!m_sOn.compare("BOND_IS_UP"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsUp()));
		}
		else if(!m_sOn.compare("BOND_IS_DOWN"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsDown()));
		}
		else if(!m_sOn.compare("BOND_IS_HASH"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsHash()));
		}
		else if(!m_sOn.compare("BOND_IS_WEDGE"))
		{
			return(ProcessBool(pRelationBond->m_pBond->IsWedge()));
		}

		return(false);
	}


	//--
    bool ObjectPropertyCondition::VSatisfy(RelationRing* pRelationRing)
	{        
		if(!m_sOn.compare("RING_SIZE"))
		{
			return(ProcessInt(pRelationRing->m_pRing->Size()));
		}
		else if(!m_sOn.compare("RING_IS_AROMATIC"))
		{
			return(ProcessBool(pRelationRing->m_pRing->IsAromatic()));
		}
		else if(!m_sOn.compare("RING_IS_HOMOCYCLIC"))
		{
			OpenBabel::OBMol* pMolecule = pRelationRing->m_pRing->GetParent();
			OpenBabel::OBAtom* pAtom;

			bool bHomoCyclic = true;

			std::vector<int>::iterator iter_path = pRelationRing->m_pRing->_path.begin();
			while(iter_path != pRelationRing->m_pRing->_path.end())
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
			OpenBabel::OBMol* pMolecule = pRelationRing->m_pRing->GetParent();
			OpenBabel::OBAtom* pAtom;

			bool bHeteroCyclic = false;

			std::vector<int>::iterator iter_path = pRelationRing->m_pRing->_path.begin();
			while(iter_path != pRelationRing->m_pRing->_path.end())
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
	bool ObjectPropertyCondition::ProcessInt(int iValue)
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
	bool ObjectPropertyCondition::ProcessBool(bool bValue)
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
	bool ObjectPropertyCondition::ProcessString(const std::string& sValue)
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
	bool ObjectPropertyCondition::ProcessDouble(double dValue)
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