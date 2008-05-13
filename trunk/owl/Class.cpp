#include "Class.h"

namespace Owl
{
	//--
	Class::Class() : m_sPrefix(""), m_sName(""), m_ptrRoot(NULL)
	{
		m_ptrRoot = new ClassIntersection();

	}



	//--
	Class::Class(const std::string& refPrefix, const std::string& refName) : m_sPrefix(refPrefix), m_sName(refName), m_ptrRoot(NULL)
	{
		m_ptrRoot = new ClassIntersection();
	}


	//--
	Class::~Class()
	{
		if(m_ptrRoot)
		{
			delete(m_ptrRoot);
			m_ptrRoot = NULL;
		}
	}


	//--
	const std::string& Class::GetPrefix() const
	{
		return(m_sPrefix);
	}



	//--
	const std::string& Class::GetName() const
	{
		return(m_sName);
	}




	//--
	void Class::UnionStart()
	{
		m_ptrRoot->UnionStart();
	}




	//--
	void Class::UnionEnd()
	{
		m_ptrRoot->UnionEnd();
	}




	//--
	void Class::IntersectionStart()
	{
		m_ptrRoot->IntersectionStart();
	}




	//--
	void Class::IntersectionEnd()
	{
		m_ptrRoot->IntersectionEnd();
	}



	//--
	void Class::AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
	{
		m_ptrRoot->AddCondition(refOn, refValue, refOperation);
	}




	//--
	void Class::RetrieveClass(std::set<std::string>& refClasses)
	{
		std::string sBuf = m_sPrefix + ";" + m_sName;
		refClasses.insert(sBuf);
	}


	//--
	void Class::Print(std::ostream* pOutputStream)
	{
		*pOutputStream << "<owl:Class rdf:about=\"&" << m_sPrefix << ";" << m_sName << "\" />" << std::endl;

		*pOutputStream << std::endl << std::endl;
	}



	//--
	// [rad] attempt to satisfy descriptor with this class
	bool Class::SatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux)
	{
		/*
		// [rad] quick check to see if this is the correct class for this descriptor
		// based on auxiliary value
		std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
		std::string sQualityName, sQualityValue;
		OpenBabel::OBPairData* pAuxData;

		bool bFound = false;
		

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
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR") && !refAux.compare("PUBCHEM_CACTVS_HBOND_ACCEPTOR"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_HBOND_DONOR") && !refAux.compare("PUBCHEM_CACTVS_HBOND_DONOR"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_ROTATABLE_BOND") && !refAux.compare("PUBCHEM_CACTVS_ROTATABLE_BOND"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_SUBSKEYS") && !refAux.compare("PUBCHEM_CACTVS_SUBSKEYS"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_OPENEYE_NAME") && !refAux.compare("PUBCHEM_IUPAC_OPENEYE_NAME"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_CAS_NAME") && !refAux.compare("PUBCHEM_IUPAC_CAS_NAME"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_NAME") && !refAux.compare("PUBCHEM_IUPAC_NAME"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME") && !refAux.compare("PUBCHEM_IUPAC_SYSTEMATIC_NAME"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME") && !refAux.compare("PUBCHEM_IUPAC_TRADITIONAL_NAME"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_NIST_INCHI") && !refAux.compare("PUBCHEM_NIST_INCHI"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_XLOGP") && !refAux.compare("PUBCHEM_CACTVS_XLOGP"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_EXACT_MASS") && !refAux.compare("PUBCHEM_CACTVS_EXACT_MASS"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MF") && !refAux.compare("PUBCHEM_OPENEYE_MF"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MW") && !refAux.compare("PUBCHEM_OPENEYE_MW"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_CAN_SMILES") && !refAux.compare("PUBCHEM_OPENEYE_CAN_SMILES"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_ISO_SMILES") && !refAux.compare("PUBCHEM_OPENEYE_ISO_SMILES"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TPSA") && !refAux.compare("PUBCHEM_CACTVS_TPSA"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT") && !refAux.compare("PUBCHEM_OPENEYE_MONOISOTOPICWT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_TOTAL_CHARGE") && !refAux.compare("PUBCHEM_TOTAL_CHARGE"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_HEAVY_ATOM_COUNT") && !refAux.compare("PUBCHEM_HEAVY_ATOM_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_ATOM_DEF_STEREO_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_ATOM_UDEF_STEREO_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_DEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_BOND_DEF_STEREO_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT") && !refAux.compare("PUBCHEM_BOND_UDEF_STEREO_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT") && !refAux.compare("PUBCHEM_ISOTOPIC_ATOM_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_COMPONENT_COUNT") && !refAux.compare("PUBCHEM_COMPONENT_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_TAUTO_COUNT") && !refAux.compare("PUBCHEM_CACTVS_TAUTO_COUNT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_BONDANNOTATIONS") && !refAux.compare("PUBCHEM_BONDANNOTATIONS"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_COMPOUND_CANONICALIZED") && !refAux.compare("PUBCHEM_COMPOUND_CANONICALIZED"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_CACTVS_COMPLEXITY") && !refAux.compare("PUBCHEM_CACTVS_COMPLEXITY"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_FORMULA") && !refAux.compare("PUBCHEM_MOLECULAR_FORMULA"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_MOLECULAR_WEIGHT") && !refAux.compare("PUBCHEM_MOLECULAR_WEIGHT"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_EXACT_MASS") && !refAux.compare("PUBCHEM_EXACT_MASS"))
				{
					bFound = true;
				}
				else if(!sQualityName.compare("PUBCHEM_MONOISOTOPIC_WEIGHT") && !refAux.compare("PUBCHEM_MONOISOTOPIC_WEIGHT"))
				{
					bFound = true;
				}
			}

		}
		
		if(!bFound) return(false);
		*/
		
		return(m_ptrRoot->SatisfyDescriptor(pMolecule, refAux));
	}
	
	
		
	//--
	// [rad] attempt to satisfy molecule with this class
	bool Class::Satisfy(OpenBabel::OBMol* pMolecule)
	{
		return(m_ptrRoot->Satisfy(pMolecule));
	}



	//--
	// [rad] attempt to satisfy atom with this class
	bool Class::Satisfy(OpenBabel::OBAtom* pAtom)
	{
		return(m_ptrRoot->Satisfy(pAtom));
	}



	//--
	// [rad] attempt to satisfy bond with this class
	bool Class::Satisfy(OpenBabel::OBBond* pBond)
	{
		return(m_ptrRoot->Satisfy(pBond));
	}


	//--
	// [rad] attempt to satisfy ring with this class
	bool Class::Satisfy(OpenBabel::OBRing* pRing)
	{
		return(m_ptrRoot->Satisfy(pRing));
	}


	//--
	bool Class::IsDefault() const
	{
		return(m_ptrRoot->IsDefault());
	}
}