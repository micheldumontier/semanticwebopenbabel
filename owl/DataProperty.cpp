#include "DataProperty.h"

namespace Owl
{
	//--
	DataProperty::DataProperty() : m_sPrefix(""), m_sName(""), m_sType("")
	{
	}



	//--
	DataProperty::DataProperty(const std::string& refPrefix, const std::string& refName, const std::string& refType) 
		: m_sPrefix(refPrefix), m_sName(refName), m_sType(refType)
	{
	}



	//--
	DataProperty::~DataProperty()
	{
		std::vector<DataPropertyOption*>::iterator iter_dataprop_entries = m_vecOptions.begin();
		while(m_vecOptions.end() != iter_dataprop_entries)
		{
			delete(*iter_dataprop_entries);
			
			// [rad] to next entry
			iter_dataprop_entries++;
		}
	
		m_vecOptions.clear();

	}
	
	
	

	//--
	const std::string& DataProperty::GetPrefix() const
	{
		return(m_sPrefix);
	}



	//--
	const std::string& DataProperty::GetName() const
	{
		return(m_sName);
	}



	//--
	const std::string& DataProperty::GetType() const
	{
		return(m_sType);
	}



	//--
	bool DataProperty::IsDefault() const
	{
		return(!m_vecOptions.size());
	}



	//--
	void DataProperty::AddOption(const std::string& refPrefix, const std::string& refName)
	{
		m_vecOptions.push_back(new DataPropertyOption(refPrefix, refName));
	}



	//--
	void DataProperty::RetrieveProperty(std::set<std::string>& refDataProperties)
	{
		std::string sBuf = m_sPrefix + ";" + m_sName;
		refDataProperties.insert(sBuf);
	}



	//--
	void DataProperty::Print(std::ostream* pOutputStream)
	{	/*
		if(!m_vecOptions.size())
		{
			// [rad] ignore defaults for now
			return;
		}
		
		*pOutputStream << "<owl:DatatypeProperty rdf:about=\"&" << m_sPrefix << ";" << m_sName << "\">" << std::endl;

		*pOutputStream << "    <rdfs:range rdf:resource=\"&xsd;" << m_sType << "\" />" << std::endl;

		if(1 == m_vecOptions.size())
		{
			//*pOutputStream << "    <rdfs:domain rdf:resource=\"&" << m_sPrefix << ";" << m_sName << "\" />" << std::endl;
			m_vecOptions.front()->Print(pOutputStream);
		}
		else
		{
			*pOutputStream << "    <rdfs:domain>" << std::endl;
			*pOutputStream << "        <owl:Class>" << std::endl;
			*pOutputStream << "            <owl:unionOf rdf:parseType=\"Collection\">" << std::endl;

			for(int i = 0; i < m_vecOptions.size(); i++)
			{
				m_vecOptions[i]->Print(pOutputStream, 4);
			}

			*pOutputStream << "            </owl:unionOf>" << std::endl;
			*pOutputStream << "        </owl:Class>" << std::endl;
			*pOutputStream << "    </rdfs:domain>" << std::endl;
		}		
		
		*pOutputStream << "</owl:DatatypeProperty>" << std::endl;
		*pOutputStream << std::endl << std::endl;
		*/

		*pOutputStream << "<owl:DatatypeProperty rdf:about=\"&" << m_sPrefix << ";" << m_sName << "\" />" << std::endl;
		*pOutputStream << std::endl << std::endl;
	}




	//--
	void DataProperty::ConvertInt(int iValue, std::string& refValue)
	{
		std::stringstream ssConv;
		ssConv << iValue;
		ssConv >> refValue;
	}



	//--
	void DataProperty::ConvertBool(bool bValue, std::string& refValue)
	{
		if(bValue) refValue = "true";
		else refValue = "false";
	}



	//--
	void DataProperty::ConvertDouble(double dValue, std::string& refValue)
	{
		std::stringstream ssConv;
		ssConv << dValue;
		ssConv >> refValue;
	}


	//--
	// [rad] go through class defs and see which one applies to us, if there's such, store it's position
	bool DataProperty::SatisfyCommon(std::vector<Class*>& vecSatisfiedClasses, int& iPosition)
	{
		bool bSatisfied = false;

		// [rad] non-default, we need to check options
		for(int i = 0; i < vecSatisfiedClasses.size(); i++)
		{
			/*
			if(!vecSatisfiedClasses[i]->GetPrefix().compare(m_sPrefix) && !vecSatisfiedClasses[i]->GetName().compare(m_sName))
			{
				bSatisfied = true;
				iPosition = i;

				break;
			}*/

			for(int j = 0; j < m_vecOptions.size(); j++)
			{
				//if(!m_vecOptions[j]->GetPrefix().compare(m_sPrefix) && m_vecOptions[j]->GetName().compare(m_sName))
				if(!m_vecOptions[j]->GetPrefix().compare(vecSatisfiedClasses[i]->GetPrefix()) && 
					!m_vecOptions[j]->GetName().compare(vecSatisfiedClasses[i]->GetName()))
				{
					bSatisfied = true;
					iPosition = i;

					break;
				}
			}

			
			if(bSatisfied) break;
		}

		return(bSatisfied);
	}



	//--
	bool DataProperty::SatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refTo, std::string& refValue)
	{
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;


		for(iter_data = pMolecule->BeginData(); iter_data != pMolecule->EndData(); iter_data++)
		{
			// [rad] Take care of PairData only for now
			if(1 == (*iter_data)->GetDataType())
			{
				pAuxData = (OpenBabel::OBPairData*)(*iter_data);
				sQualityName = pAuxData->GetAttribute();
				sQualityValue = pAuxData->GetValue();

				if(!sQualityName.compare("PUBCHEM_COMPOUND_CID") && !refTo.compare("MOLECULE_PUBCHEM_COMPOUND_CID")) 
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_HBOND_ACCEPTOR"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_DONOR") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_HBOND_DONOR"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_ROTATABLE_BOND") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_ROTATABLE_BOND"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_SUBSKEYS") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_SUBSKEYS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_OPENEYE_NAME") && !refTo.compare("MOLECULE_PUBCHEM_IUPAC_OPENEYE_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_CAS_NAME") && !refTo.compare("MOLECULE_PUBCHEM_IUPAC_CAS_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_NAME") && !refTo.compare("MOLECULE_PUBCHEM_IUPAC_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME") && !refTo.compare("MOLECULE_PUBCHEM_IUPAC_SYSTEMATIC_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME") && !refTo.compare("MOLECULE_PUBCHEM_IUPAC_TRADITIONAL_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_NIST_INCHI") && !refTo.compare("MOLECULE_PUBCHEM_NIST_INCHI"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_XLOGP") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_XLOGP"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_EXACT_MASS") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_EXACT_MASS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MF") && !refTo.compare("MOLECULE_PUBCHEM_OPENEYE_MF"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MW") && !refTo.compare("MOLECULE_PUBCHEM_OPENEYE_MW"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_CAN_SMILES") && !refTo.compare("MOLECULE_PUBCHEM_OPENEYE_CAN_SMILES"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_ISO_SMILES") && !refTo.compare("MOLECULE_PUBCHEM_OPENEYE_ISO_SMILES"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TPSA") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_TPSA"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT") && !refTo.compare("MOLECULE_PUBCHEM_OPENEYE_MONOISOTOPICWT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_TOTAL_CHARGE") && !refTo.compare("MOLECULE_PUBCHEM_TOTAL_CHARGE"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_HEAVY_ATOM_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_HEAVY_ATOM_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_ATOM_DEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_ATOM_UDEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_DEF_STEREO_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_BOND_DEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_BOND_UDEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_ISOTOPIC_ATOM_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_COMPONENT_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_COMPONENT_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TAUTO_COUNT") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_TAUTO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_BONDANNOTATIONS") && !refTo.compare("MOLECULE_PUBCHEM_BONDANNOTATIONS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_COMPOUND_CANONICALIZED") && !refTo.compare("MOLECULE_PUBCHEM_COMPOUND_CANONICALIZED"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_COMPLEXITY") && !refTo.compare("MOLECULE_PUBCHEM_CACTVS_COMPLEXITY"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_FORMULA") && !refTo.compare("MOLECULE_PUBCHEM_MOLECULAR_FORMULA"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_WEIGHT") && !refTo.compare("MOLECULE_PUBCHEM_MOLECULAR_WEIGHT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_EXACT_MASS") && !refTo.compare("MOLECULE_PUBCHEM_EXACT_MASS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_MONOISOTOPIC_WEIGHT") && !refTo.compare("MOLECULE_PUBCHEM_MONOISOTOPIC_WEIGHT"))
				{
					refValue = sQualityValue;
					return(true);
				}
			}

		}

		return(false);
	}






	//--
	bool DataProperty::SatisfyDirectDescriptor(OpenBabel::OBMol* pMolecule, 
									const std::string& refTo, 
									std::vector<Class*>& vecSatisfiedClasses, 
									std::string& refValue)
	{

		int iPosition = -1;
		
		// [rad] if we are default, we automatically satisfy		
		if(!IsDefault())
		{			
			if(!SatisfyCommon(vecSatisfiedClasses, iPosition))
			{
				// [rad] this property does not apply

				refValue = "";
				return(false);
			}
		}
		
		
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;


		for(iter_data = pMolecule->BeginData(); iter_data != pMolecule->EndData(); iter_data++)
		{
			// [rad] Take care of PairData only for now
			if(1 == (*iter_data)->GetDataType())
			{
				pAuxData = (OpenBabel::OBPairData*)(*iter_data);
				sQualityName = pAuxData->GetAttribute();
				sQualityValue = pAuxData->GetValue();
				
			

				if(!sQualityName.compare("PUBCHEM_COMPOUND_CID") && !refTo.compare("PUBCHEM_COMPOUND_CID")) 
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR") && !refTo.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_DONOR") && !refTo.compare("PUBCHEM_CACTVS_HBOND_DONOR"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_ROTATABLE_BOND") && !refTo.compare("PUBCHEM_CACTVS_ROTATABLE_BOND"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_SUBSKEYS") && !refTo.compare("PUBCHEM_CACTVS_SUBSKEYS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_OPENEYE_NAME") && !refTo.compare("PUBCHEM_IUPAC_OPENEYE_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_CAS_NAME") && !refTo.compare("PUBCHEM_IUPAC_CAS_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_NAME") && !refTo.compare("PUBCHEM_IUPAC_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME") && !refTo.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME") && !refTo.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_NIST_INCHI") && !refTo.compare("PUBCHEM_NIST_INCHI"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_XLOGP") && !refTo.compare("PUBCHEM_CACTVS_XLOGP"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_EXACT_MASS") && !refTo.compare("PUBCHEM_CACTVS_EXACT_MASS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MF") && !refTo.compare("PUBCHEM_OPENEYE_MF"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MW") && !refTo.compare("PUBCHEM_OPENEYE_MW"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_CAN_SMILES") && !refTo.compare("PUBCHEM_OPENEYE_CAN_SMILES"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_ISO_SMILES") && !refTo.compare("PUBCHEM_OPENEYE_ISO_SMILES"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TPSA") && !refTo.compare("PUBCHEM_CACTVS_TPSA"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT") && !refTo.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_TOTAL_CHARGE") && !refTo.compare("PUBCHEM_TOTAL_CHARGE"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_HEAVY_ATOM_COUNT") && !refTo.compare("PUBCHEM_HEAVY_ATOM_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT") && !refTo.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT") && !refTo.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_DEF_STEREO_COUNT") && !refTo.compare("PUBCHEM_BOND_DEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT") && !refTo.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT") && !refTo.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_COMPONENT_COUNT") && !refTo.compare("PUBCHEM_COMPONENT_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TAUTO_COUNT") && !refTo.compare("PUBCHEM_CACTVS_TAUTO_COUNT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_BONDANNOTATIONS") && !refTo.compare("PUBCHEM_BONDANNOTATIONS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_COMPOUND_CANONICALIZED") && !refTo.compare("PUBCHEM_COMPOUND_CANONICALIZED"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_COMPLEXITY") && !refTo.compare("PUBCHEM_CACTVS_COMPLEXITY"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_FORMULA") && !refTo.compare("PUBCHEM_MOLECULAR_FORMULA"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_WEIGHT") && !refTo.compare("PUBCHEM_MOLECULAR_WEIGHT"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_EXACT_MASS") && !refTo.compare("PUBCHEM_EXACT_MASS"))
				{
					refValue = sQualityValue;
					return(true);
				}
				else if(!sQualityName.compare("PUBCHEM_MONOISOTOPIC_WEIGHT") && !refTo.compare("PUBCHEM_MONOISOTOPIC_WEIGHT"))
				{
					refValue = sQualityValue;
					return(true);
				}
			}

		}

		return(false);

	}
									
									
									
									
									
									
	//--
	bool DataProperty::Satisfy(OpenBabel::OBMol* pMolecule, 
									const std::string& refTo, 
									std::vector<Class*>& vecSatisfiedClasses, 
									std::string& refValue)
	{
		//bool bSatisfied = false;
		int iPosition = -1;

		// [rad] if we are default, we automatically satisfy		
		if(!IsDefault())
		{			
			if(!SatisfyCommon(vecSatisfiedClasses, iPosition))
			{
				// [rad] this property does not apply

				refValue = "";
				return(false);
			}
		}

		// [rad] go through possible data types..

		if(!refTo.compare("MOLECULE_ATOM_COUNT"))
		{
			ConvertInt(pMolecule->NumAtoms(), refValue);
		}
		else if(!refTo.compare("MOLECULE_BOND_COUNT"))
		{
			ConvertInt(pMolecule->NumBonds(), refValue);
		}
		else if(!refTo.compare("MOLECULE_RING_COUNT"))
		{
			std::vector<OpenBabel::OBRing*>& refSSSR = pMolecule->GetSSSR();
			ConvertInt(refSSSR.size(), refValue);
		}
		else if(!refTo.compare("MOLECULE_HEAVY_HYDROGEN_COUNT"))
		{
			ConvertInt(pMolecule->NumHvyAtoms(), refValue);
		}
		else if(!refTo.compare("MOLECULE_RESIDUE_COUNT"))
		{
			ConvertInt(pMolecule->NumResidues(), refValue);
		}
		else if(!refTo.compare("MOLECULE_ROTOR_COUNT"))
		{
			ConvertInt(pMolecule->NumRotors(), refValue);
		}
		else if(!refTo.compare("MOLECULE_FORMULA"))
		{
			refValue = pMolecule->GetFormula();
		}
		else if(!refTo.compare("MOLECULE_FORMATION_HEAT"))
		{
			ConvertDouble(pMolecule->GetEnergy(), refValue);
		}
		else if(!refTo.compare("MOLECULE_STANDARD_MOLAR_MASS"))
		{
			ConvertDouble(pMolecule->GetMolWt(), refValue);
		}
		else if(!refTo.compare("MOLECULE_EXACT_MASS"))
		{
			ConvertDouble(pMolecule->GetExactMass(), refValue);
		}
		else if(!refTo.compare("MOLECULE_TOTAL_CHARGE"))
		{
			ConvertInt(pMolecule->GetTotalCharge(), refValue);
		}
		else if(!refTo.compare("MOLECULE_SPIN_MULTIPLICITY"))
		{
			ConvertInt(pMolecule->GetTotalSpinMultiplicity(), refValue);
		}
		else if(!refTo.compare("MOLECULE_IS_CHIRAL"))
		{
			ConvertBool(pMolecule->IsChiral(), refValue);
		}
		else if(!refTo.compare("MOLECULE_HAS_AROMATIC_RING"))
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

			ConvertBool(bAromatic, refValue);
		}
		else if(!refTo.compare("MOLECULE_HAS_HOMOCYCLIC_RING"))
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
		
			ConvertBool(bHomoCyclic, refValue);
		}
		else if(!refTo.compare("MOLECULE_HAS_HETEROCYCLIC_RING"))
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
		
			ConvertBool(bHeteroCyclic, refValue);
		}
		else
		{
			// [rad] maybe it's a descriptor?
			if(SatisfyDescriptor(pMolecule, refTo, refValue))
			{
				return(true);
			}

			// [rad] unknown datatype?
			refValue = "";
			return(false);
		}

		return(true);
	}



	//--
	bool DataProperty::Satisfy(OpenBabel::OBAtom* pAtom, 
								const std::string& refTo, 
								std::vector<Class*>& vecSatisfiedClasses, 
								std::string& refValue)
	{
		//bool bSatisfied = false;
		int iPosition = -1;

		// [rad] if we are default, we automatically satisfy		
		if(!IsDefault())
		{			
			if(!SatisfyCommon(vecSatisfiedClasses, iPosition))
			{
				// [rad] this property does not apply

				refValue = "";
				return(false);
			}
		}

		// [rad] go through possible data types..

		if(!refTo.compare("ATOM_ATOMIC_NUMBER"))
		{
			ConvertInt(pAtom->GetAtomicNum(), refValue);
		}
		else if(!refTo.compare("ATOM_FORMAL_CHARGE"))
		{
			ConvertInt(pAtom->GetFormalCharge(), refValue);
		}
		else if(!refTo.compare("ATOM_ISOTOPE"))
		{
			ConvertInt(pAtom->GetIsotope(), refValue);
		}
		else if(!refTo.compare("ATOM_SPIN_MULTIPLICITY"))
		{
			ConvertInt(pAtom->GetSpinMultiplicity(), refValue);
		}
		else if(!refTo.compare("ATOM_ATOMIC_MASS"))
		{
			ConvertDouble(pAtom->GetAtomicMass(), refValue);
		}
		else if(!refTo.compare("ATOM_EXACT_MASS"))
		{
			ConvertDouble(pAtom->GetExactMass(), refValue);
		}
		else if(!refTo.compare("ATOM_INTERNAL_INDEX"))
		{
			ConvertInt(pAtom->GetIdx(), refValue);
		}
		else if(!refTo.compare("ATOM_VALENCE"))
		{
			ConvertInt(pAtom->GetValence(), refValue);
		}
		else if(!refTo.compare("ATOM_HYBRIDIZATION"))
		{
			ConvertInt(pAtom->GetHyb(), refValue);
		}
		else if(!refTo.compare("ATOM_IMPLICIT_VALENCE"))
		{
			ConvertInt(pAtom->GetImplicitValence(), refValue);
		}
		else if(!refTo.compare("ATOM_HEAVY_VALENCE"))
		{
			ConvertInt(pAtom->GetHvyValence(), refValue);
		}
		else if(!refTo.compare("ATOM_HETERO_VALENCE"))
		{
			ConvertInt(pAtom->GetHeteroValence(), refValue);
		}
		else if(!refTo.compare("ATOM_COORDINATE_X"))
		{
			ConvertDouble(pAtom->GetX(), refValue);
		}
		else if(!refTo.compare("ATOM_COORDINATE_Y"))
		{
			ConvertDouble(pAtom->GetY(), refValue);
		}
		else if(!refTo.compare("ATOM_COORDINATE_Z"))
		{
			ConvertDouble(pAtom->GetZ(), refValue);
		}
		else if(!refTo.compare("ATOM_PARTIAL_CHARGE"))
		{
			ConvertDouble(pAtom->GetPartialCharge(), refValue);
		}
		else if(!refTo.compare("ATOM_FREE_OXYGEN_COUNT"))
		{
			ConvertInt(pAtom->CountFreeOxygens(), refValue);
		}
		else if(!refTo.compare("ATOM_IMPLICIT_HYDROGEN_COUNT"))
		{
			ConvertInt(pAtom->ImplicitHydrogenCount(), refValue);
		}
		else if(!refTo.compare("ATOM_PARTICIPANT_RING_COUNT"))
		{
			ConvertInt(pAtom->MemberOfRingCount(), refValue);
		}
		else if(!refTo.compare("ATOM_AVERAGE_BOND_ANGLE"))
		{
			ConvertDouble(pAtom->AverageBondAngle(), refValue);
		}
		else if(!refTo.compare("ATOM_SMALLEST_BOND_ANGLE"))
		{
			ConvertDouble(pAtom->SmallestBondAngle(), refValue);
		}
		else if(!refTo.compare("ATOM_IS_IN_RING"))
		{
			ConvertBool(pAtom->IsInRing(), refValue);
		}
		else if(!refTo.compare("ATOM_IS_HETERO_ATOM"))
		{
			ConvertBool(pAtom->IsHeteroatom(), refValue);
		}
		else if(!refTo.compare("ATOM_IS_AROMATIC"))
		{
			ConvertBool(pAtom->IsAromatic(), refValue);
		}
		else if(!refTo.compare("ATOM_IS_CHIRAL"))
		{
			ConvertBool(pAtom->IsChiral(), refValue);
		}
		else if(!refTo.compare("ATOM_HAS_SINGLE_BOND"))
		{
			ConvertBool(pAtom->HasSingleBond(), refValue);
		}
		else if(!refTo.compare("ATOM_HAS_DOUBLE_BOND"))
		{
			ConvertBool(pAtom->HasDoubleBond(), refValue);
		}
		else if(!refTo.compare("ATOM_HAS_TRIPLE_BOND"))
		{
			ConvertBool(pAtom->HasBondOfOrder(3), refValue);
		}		
		else if(!refTo.compare("ATOM_HAS_AROMATIC_BOND"))
		{
			ConvertBool(pAtom->HasAromaticBond(), refValue);
		}		
		else if(!refTo.compare("ATOM_SINGLE_BOND_COUNT"))
		{
			ConvertInt(pAtom->CountBondsOfOrder(1), refValue);
		}
		else if(!refTo.compare("ATOM_DOUBLE_BOND_COUNT"))
		{
			ConvertInt(pAtom->CountBondsOfOrder(2), refValue);
		}
		else if(!refTo.compare("ATOM_TRIPLE_BOND_COUNT"))
		{
			ConvertInt(pAtom->CountBondsOfOrder(3), refValue);
		}
		else if(!refTo.compare("ATOM_AROMATIC_BOND_COUNT"))
		{
			ConvertInt(pAtom->CountBondsOfOrder(5), refValue);
		}
		else if(!refTo.compare("ATOM_BOND_COUNT"))
		{
			//return(ConvertInt(pAtom->CountBondsOfOrder(5)));
			OpenBabel::OBBond* pBond;
			std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bond;
			int iCount = 0;

			for(pBond = pAtom->BeginBond(iter_bond); pBond; pBond = pAtom->NextBond(iter_bond))
			{
				iCount++;
			}
	
			ConvertInt(iCount, refValue);
		}
		else
		{
			// [rad] unknown datatype?
			refValue = "";
			return(false);
		}

		return(true);
	}




	//--
	bool DataProperty::Satisfy(OpenBabel::OBBond* pBond, 
								const std::string& refTo, 
								std::vector<Class*>& vecSatisfiedClasses, 
								std::string& refValue)
	{
		//bool bSatisfied = false;
		int iPosition = -1;

		// [rad] if we are default, we automatically satisfy		
		if(!IsDefault())
		{			
			if(!SatisfyCommon(vecSatisfiedClasses, iPosition))
			{
				// [rad] this property does not apply

				refValue = "";
				return(false);
			}
		}

		// [rad] go through possible data types..

		if(!refTo.compare("BOND_ORDER"))
		{
			ConvertInt(pBond->GetBondOrder(), refValue);
		}
		else if(!refTo.compare("BOND_EQUILIBRIUM_LENGTH"))
		{
			ConvertDouble(pBond->GetEquibLength(), refValue);
		}
		else if(!refTo.compare("BOND_LENGTH"))
		{
			ConvertDouble(pBond->GetLength(), refValue);
		}
		else if(!refTo.compare("BOND_IS_AROMATIC"))
		{
			ConvertBool(pBond->IsAromatic(), refValue);
		}
		else if(!refTo.compare("BOND_IS_IN_RING"))
		{
			ConvertBool(pBond->IsInRing(), refValue);
		}
		else if(!refTo.compare("BOND_IS_ROTOR"))
		{
			ConvertBool(pBond->IsRotor(), refValue);
		}
		else if(!refTo.compare("BOND_IS_AMIDE"))
		{
			ConvertBool(pBond->IsAmide(), refValue);
		}
		else if(!refTo.compare("BOND_IS_PRIMARY_AMIDE"))
		{
			ConvertBool(pBond->IsPrimaryAmide(), refValue);
		}
		else if(!refTo.compare("BOND_IS_SECONDARY_AMIDE"))
		{
			ConvertBool(pBond->IsSecondaryAmide(), refValue);
		}
		else if(!refTo.compare("BOND_IS_ESTER"))
		{
			ConvertBool(pBond->IsEster(), refValue);
		}
		else if(!refTo.compare("BOND_IS_CARBONYL"))
		{
			ConvertBool(pBond->IsCarbonyl(), refValue);
		}
		else if(!refTo.compare("BOND_IS_SINGLE_BOND"))
		{
			ConvertBool(pBond->IsSingle(), refValue);
		}
		else if(!refTo.compare("BOND_IS_DOUBLE_BOND"))
		{
			ConvertBool(pBond->IsDouble(), refValue);
		}
		else if(!refTo.compare("BOND_IS_TRIPLE_BOND"))
		{
			ConvertBool(pBond->IsTriple(), refValue);
		}
		else if(!refTo.compare("BOND_IS_CLOSURE_BOND"))
		{
			ConvertBool(pBond->IsClosure(), refValue);
		}
		else if(!refTo.compare("BOND_IS_UP"))
		{
			ConvertBool(pBond->IsUp(), refValue);
		}
		else if(!refTo.compare("BOND_IS_DOWN"))
		{
			ConvertBool(pBond->IsDown(), refValue);
		}
		else if(!refTo.compare("BOND_IS_HASH"))
		{
			ConvertBool(pBond->IsHash(), refValue);
		}
		else if(!refTo.compare("BOND_IS_WEDGE"))
		{
			ConvertBool(pBond->IsWedge(), refValue);
		}
		else
		{
			// [rad] unknown datatype?
			refValue = "";
			return(false);
		}

		return(true);
	}




	//--
	bool DataProperty::Satisfy(OpenBabel::OBRing* pRing, 
								const std::string& refTo, 
								std::vector<Class*>& vecSatisfiedClasses, 
								std::string& refValue)
	{
		//bool bSatisfied = false;
		int iPosition = -1;

		// [rad] if we are default, we automatically satisfy		
		if(!IsDefault())
		{			
			if(!SatisfyCommon(vecSatisfiedClasses, iPosition))
			{
				// [rad] this property does not apply

				refValue = "";
				return(false);
			}
		}

		// [rad] go through possible data types..

		if(!refTo.compare("RING_SIZE"))
		{
			ConvertInt(pRing->Size(), refValue);
		}
		else if(!refTo.compare("RING_IS_AROMATIC"))
		{
			ConvertBool(pRing->IsAromatic(), refValue);
		}
		else if(!refTo.compare("RING_IS_HOMOCYCLIC"))
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

			ConvertBool(bHomoCyclic, refValue);
		}
		else if(!refTo.compare("RING_IS_HETEROCYCLIC"))
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

			ConvertBool(bHeteroCyclic, refValue);
		}
		else
		{
			// [rad] unknown datatype?
			refValue = "";
			return(false);
		}

		return(true);
	}
}