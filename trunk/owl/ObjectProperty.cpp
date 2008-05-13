#include "ObjectProperty.h"
#include "Relation.h"
#include "RelationBond.h"
#include "RelationAtom.h"
#include "RelationMolecule.h"
#include "RelationRing.h"
#include "RelationDescriptor.h"

namespace Owl
{
	//--
	ObjectProperty::ObjectProperty() : m_sPrefix(""), m_sName(""), m_sAuxiliary(""), 
		m_bDomainCurrent(false), m_bRangeCurrent(false), m_ptrDomain(NULL), m_ptrRange(NULL)
	{
	}



	//--
	ObjectProperty::ObjectProperty(const std::string& refPrefix, const std::string& refName, const std::string& refAuxiliary) 
		: m_sPrefix(refPrefix), m_sName(refName), m_sAuxiliary(refAuxiliary), m_bDomainCurrent(false), m_bRangeCurrent(false),
		m_ptrDomain(NULL), m_ptrRange(NULL)
	{
	}



	//--
	ObjectProperty::~ObjectProperty()
	{
		if(m_ptrRange)
		{
			delete(m_ptrRange);
			m_ptrRange = NULL;
		}
		
		if(m_ptrDomain)
		{
			delete(m_ptrDomain);
			m_ptrDomain = NULL;
		}
	}
	
	
	

	//--
	const std::string& ObjectProperty::GetPrefix() const
	{
		return(m_sPrefix);
	}



	//--
	const std::string& ObjectProperty::GetName() const
	{
		return(m_sName);
	}


	//--
	const std::string& ObjectProperty::GetAuxiliary() const
	{
		return(m_sAuxiliary);
	}




	//--
	bool ObjectProperty::IsDefault() const
	{
		return(!m_ptrDomain && !m_ptrRange);
	}




	//--
	void ObjectProperty::AddDomain(const std::string& refPrefix, const std::string& refName)
	{
		if(!m_ptrDomain)
		{
			m_ptrDomain = new ObjectPropertyDomain(refPrefix, refName);

			m_bRangeCurrent = false;
			m_bDomainCurrent = true;
		}
	}




	//--
	void ObjectProperty::AddRange(const std::string& refPrefix, const std::string& refName)
	{
		if(!m_ptrRange)
		{
			m_ptrRange = new ObjectPropertyRange(refPrefix, refName);

			m_bRangeCurrent = true;
			m_bDomainCurrent = false;
		}
	}



	//--
	void ObjectProperty::AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
	{
		if(m_bDomainCurrent && !m_bRangeCurrent)
		{
			m_ptrDomain->AddCondition(refOn, refValue, refOperation);
		}
		else if(m_bRangeCurrent && !m_bDomainCurrent)
		{
			m_ptrRange->AddCondition(refOn, refValue, refOperation);
		}
	}




	//--
	void ObjectProperty::IntersectionEnd()
	{
		if(m_bDomainCurrent && !m_bRangeCurrent)
		{
			m_ptrDomain->IntersectionEnd();
		}
		else if(m_bRangeCurrent && !m_bDomainCurrent)
		{
			m_ptrRange->IntersectionEnd();
		}
	}


	//--
	void ObjectProperty::IntersectionStart()
	{
		if(m_bDomainCurrent && !m_bRangeCurrent)
		{
			m_ptrDomain->IntersectionStart();
		}
		else if(m_bRangeCurrent && !m_bDomainCurrent)
		{
			m_ptrRange->IntersectionStart();
		}
	}



	//--
	void ObjectProperty::UnionStart()
	{
		if(m_bDomainCurrent && !m_bRangeCurrent)
		{
			m_ptrDomain->UnionStart();
		}
		else if(m_bRangeCurrent && !m_bDomainCurrent)
		{
			m_ptrRange->UnionStart();
		}
	}



	//--
	void ObjectProperty::UnionEnd()
	{
		if(m_bDomainCurrent && !m_bRangeCurrent)
		{
			m_ptrDomain->UnionEnd();
		}
		else if(m_bRangeCurrent && !m_bDomainCurrent)
		{
			m_ptrRange->UnionEnd();
		}
	}



	//--
	void ObjectProperty::Print(std::ostream* pOutputStream)
	{
		*pOutputStream << "<owl:ObjectProperty rdf:about=\"&" << m_sPrefix << ";" << m_sName << "\" />" << std::endl;
		*pOutputStream << std::endl << std::endl;

	}
	


	//--
	void ObjectProperty::RetrieveProperty(std::set<std::string>& refObjectProperties)
	{
		std::string sBuf = m_sPrefix + ";" + m_sName;
		refObjectProperties.insert(sBuf);
	}

	
    
	//--
    bool ObjectProperty::Satisfy(RelationMolecule* pRelMolecule, 
									RelationDescriptor* pRelDescriptor, 
									bool bForward)
    {
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelMolecule);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelDescriptor);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelDescriptor);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelMolecule);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			// [rad] check if these two are related
			OpenBabel::OBMol* pMolecule = pRelMolecule->m_pMol;
			
			OpenBabel::OBMol* pDescriptorMolecule = pRelDescriptor->m_pMol;


			if(pMolecule == pDescriptorMolecule)
			{
				return(true);
			}
		}

		return(false);
	}
	
	
	
	
	
    //--
    bool ObjectProperty::Satisfy(RelationMolecule* pRelMolecule, 
									RelationAtom* pRelAtom, 
									bool bForward)
    {
        bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelMolecule);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelMolecule);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			// [rad] check if these two are related
			OpenBabel::OBMol* pMolecule = pRelMolecule->m_pMol;
			
			OpenBabel::OBAtom* pAtomComp = pRelAtom->m_pAtom;
			
			OpenBabel::OBAtom* pAtom;

			std::vector<OpenBabel::OBNodeBase*>::iterator iter_atom;
			for(pAtom = pMolecule->BeginAtom(iter_atom); pAtom; pAtom = pMolecule->NextAtom(iter_atom))
			{
				if(pAtom == pAtomComp)
				{
					return(true);
				}
			}
			
			//return(true);
		}

		return(false);
    
    }
    
    
    
    

	//--
	bool ObjectProperty::Satisfy(RelationMolecule* pRelMolecule, RelationBond* pRelBond, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelMolecule);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelBond);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelBond);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelMolecule);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			OpenBabel::OBMol* pMolecule = pRelMolecule->m_pMol;
			OpenBabel::OBBond* pBondComp = pRelBond->m_pBond;
			
			OpenBabel::OBBond* pBond;

			std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bonds;
			for(pBond = pMolecule->BeginBond(iter_bonds); pBond; pBond = pMolecule->NextBond(iter_bonds))
			{
				if(pBond == pBondComp)
				{
					return(true);
				}
			}

			//return(true);
		}

		return(false);
	}




	//--
	bool ObjectProperty::Satisfy(RelationMolecule* pRelMolecule, RelationRing* pRelRing, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelMolecule);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelRing);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelRing);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelMolecule);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			OpenBabel::OBMol* pMolecule = pRelMolecule->m_pMol;
			OpenBabel::OBRing* pRingComp = pRelRing->m_pRing;
			
			
			OpenBabel::OBRing* pRing;

			std::vector<OpenBabel::OBRing*>& refSSSR = pMolecule->GetSSSR();
			std::vector<OpenBabel::OBRing*>::iterator iter_rings = refSSSR.begin();

			while(iter_rings != refSSSR.end())
			{
				pRing = (*iter_rings);

				if(pRing == pRingComp)
				{
					return(true);
				}

				iter_rings++;
			}

			//return(true);
		}

		return(false);
	}



	//--
	bool ObjectProperty::Satisfy(RelationAtom* pRelAtom, RelationBond* pRelBond, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelBond);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelBond);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			// [rad] check if these two are related
			
			OpenBabel::OBAtom* pAtom = pRelAtom->m_pAtom;
			OpenBabel::OBBond* pBond = pRelBond->m_pBond;
			
			
			OpenBabel::OBAtom* pAtomStart = pBond->GetBeginAtom();
			OpenBabel::OBAtom* pAtomEnd = pBond->GetEndAtom();

			if(pAtom == pAtomStart || pAtom == pAtomEnd) return(true);

			//return(true);
		}

		return(false);
	}



	//--
	bool ObjectProperty::Satisfy(RelationAtom* pRelAtom, RelationRing* pRelRing, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelRing);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelRing);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			
			OpenBabel::OBMol* pMolecule = pRelRing->m_pRing->GetParent();
			OpenBabel::OBRing* pRing = pRelRing->m_pRing;
			OpenBabel::OBAtom* pAtomCond = pRelAtom->m_pAtom;
			

			OpenBabel::OBAtom* pAtom;

			std::vector<int>::iterator iter_path = pRing->_path.begin();
			while(iter_path != pRing->_path.end())
			{
				pAtom = pMolecule->GetAtom((*iter_path));

				if(pAtom == pAtomCond)
				{
					return(true);
				}

				iter_path++;
			}

			//return(true);
		}

		return(false);
	}



	//--
	bool ObjectProperty::Satisfy(RelationBond* pRelBond, RelationRing* pRelRing, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelBond);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelRing);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelRing);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelBond);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			
			OpenBabel::OBMol* pMolecule = pRelRing->m_pRing->GetParent();
			OpenBabel::OBRing* pRing = pRelRing->m_pRing;
			OpenBabel::OBBond* pBondCond = pRelBond->m_pBond;
			
			OpenBabel::OBAtom* pAtom;
			OpenBabel::OBBond* pBond;
			std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bond;

			std::vector<int>::iterator iter_path = pRing->_path.begin();
			while(iter_path != pRing->_path.end())
			{
				pAtom = pMolecule->GetAtom((*iter_path));

				for(pBond = pAtom->BeginBond(iter_bond); pBond; pBond = pAtom->NextBond(iter_bond))
				{
					if(pBondCond == pBond)
					{
						return(true);
					}
				}

				iter_path++;
			}

			//return(true);
		}

		return(false);
	}



	//--
	bool ObjectProperty::Satisfy(RelationMolecule* pRelMolecule1, RelationMolecule* pRelMolecule2, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelMolecule1);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelMolecule2);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelMolecule2);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelMolecule1);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			return(true);
		}

		return(false);
	}





	//--
	bool ObjectProperty::Satisfy(RelationAtom* pRelAtom1, RelationAtom* pRelAtom2, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom1);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom2);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom2);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom1);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			return(true);
		}

		return(false);
	}



	//--
	bool ObjectProperty::SatisfyConnected(RelationAtom* pRelAtom, RelationAtom* pRelAtom2, bool bForward)
	{
		OpenBabel::OBBond* pBond;
		OpenBabel::OBAtom* pAnotherAtom;
		std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bonds;

		bool bFound = false;

		for(pBond = pRelAtom->m_pAtom->BeginBond(iter_bonds); pBond; pBond = pRelAtom->m_pAtom->NextBond(iter_bonds))
		{
			pAnotherAtom = pBond->GetNbrAtom(pRelAtom->m_pAtom);

			if(pAnotherAtom == pRelAtom2->m_pAtom)
			{
				if(!m_sAuxiliary.size())
				{
					// [rad] aux is not set
					bFound = true;
					break;
				}
				else if(!m_sAuxiliary.compare("1"))
				{
					if(1 == pBond->GetBondOrder())
					{
						bFound = true;
						break;
					}
				}
				else if(!m_sAuxiliary.compare("2"))
				{
					if(2 == pBond->GetBondOrder())
					{
						bFound = true;
						break;
					}
				}
				else if(!m_sAuxiliary.compare("3"))
				{
					if(3 == pBond->GetBondOrder())
					{
						bFound = true;
						break;
					}
				}
				else if(!m_sAuxiliary.compare("aromatic"))
				{
					if(pBond->IsAromatic())
					{
						bFound = true;
						break;
					}
				}
                else if(!m_sAuxiliary.compare("up"))
                {
                    if(pBond->IsUp())
                    {
                        bFound = true;
                        break;
                    }
                }
                else if(!m_sAuxiliary.compare("down"))
                {
                    if(pBond->IsDown())
                    {
                        bFound = true;
                        break;
                    }
                }
                else if(!m_sAuxiliary.compare("hash"))
                {
                    if(pBond->IsHash())
                    {
                        bFound = true;
                        break;
                    }
                }
                else if(!m_sAuxiliary.compare("wedge"))
                {
                    if(pBond->IsWedge())
                    {
                        bFound = true;
                        break;
                    }
                }
			}
		}

		// [rad] not found
		if(!bFound) return(false);

		// [rad] now continue with usual ritual
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelAtom2);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom2);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelAtom);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			return(true);
		}

		return(false);
	}




	//--
	bool ObjectProperty::Satisfy(RelationBond* pRelBond1, RelationBond* pRelBond2, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelBond1);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelBond2);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelBond2);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelBond1);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			return(true);
		}

		return(false);
	}



	//--
	bool ObjectProperty::Satisfy(RelationRing* pRelRing1, RelationRing* pRelRing2, bool bForward)
	{
		bool bSatisfiedDomain = true;
		bool bSatisfiedRange = true;

		if(m_ptrDomain)
		{
			if(bForward)
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelRing1);
			}
			else
			{
				bSatisfiedDomain = m_ptrDomain->Satisfy(pRelRing2);
			}
		}

		if(m_ptrRange)
		{
			if(bForward)
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelRing2);
			}
			else
			{
				bSatisfiedRange = m_ptrRange->Satisfy(pRelRing1);
			}
		}

		// [rad] if we satisfied both conditions, it is a valid property
		if(bSatisfiedRange && bSatisfiedDomain)
		{
			return(true);
		}

		return(false);
	}
}