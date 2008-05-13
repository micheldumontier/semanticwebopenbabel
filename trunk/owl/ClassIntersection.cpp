#include "ClassIntersection.h"
#include "ClassCondition.h"
#include "ClassUnion.h"

namespace Owl
{
	//--
	ClassIntersection::ClassIntersection() : m_pCurrentActive(NULL)
	{
	}



	//--
	const std::string&	ClassIntersection::VGetDescription() const
	{
		static std::string sDescription = "Intersection";
		return(sDescription);
	}



	//--
	ClassIntersection::~ClassIntersection()
	{
		std::vector<IClassEntry*>::iterator iter_class_entries = m_vecClassEntries.begin();
		while(m_vecClassEntries.end() != iter_class_entries)
		{
			delete(*iter_class_entries);
			
			// [rad] to next entry
			iter_class_entries++;
		}
	
		m_vecClassEntries.clear();
	}


	//--
	bool ClassIntersection::IsDefault() const
	{
		return(!m_vecClassEntries.size());
	}


	//--
	void ClassIntersection::VUnionStart()
	{
		// [rad] if we don't have active union/intersection - create it
		if(!m_pCurrentActive)
		{
			m_pCurrentActive = new ClassUnion();
			
			m_vecClassEntries.push_back(m_pCurrentActive);
		}
		else
		{
			// [rad] delegate to open union/intersection
			m_pCurrentActive->UnionStart();
		}

	}



	//--
	bool ClassIntersection::VUnionEnd()
	{
		// [rad] if we don't have active union/intersection - then we must return to parent and close "this" union (self)
		if(!m_pCurrentActive)
		{
			return(false);
		}
		else
		{
			// [rad] send it to active union/intersection
			if(m_pCurrentActive->UnionEnd())
			{
				// [rad] success means it found it's place down the chain
				return(true);
			}
			else
			{
				// [rad] failure means we have to close the current active union/intersection
				m_pCurrentActive = NULL;
				return(true);
			}			
		}
	}



	//--
	void ClassIntersection::VIntersectionStart()
	{
		// [rad] if we don't have active union/intersection - create it
		if(!m_pCurrentActive)
		{
			m_pCurrentActive = new ClassIntersection();
			
			m_vecClassEntries.push_back(m_pCurrentActive);
		}
		else
		{
			// [rad] delegate to open union/intersection
			m_pCurrentActive->IntersectionStart();
		}
	}



	//--
	bool ClassIntersection::VIntersectionEnd()
	{
		// [rad] if we don't have active union/intersection - then we must return to parent and close this union
		if(!m_pCurrentActive)
		{
			return(false);
		}
		else
		{
			// [rad] send it to active union/intersection
			if(m_pCurrentActive->IntersectionEnd())
			{
				// [rad] success means it found it's place down the chain
				return(true);
			}
			else
			{
				// [rad] failure means we have to close the current active union/intersection
				m_pCurrentActive = NULL;
				return(true);
			}			
		}
	}




	//--
	void ClassIntersection::VAddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
	{
		// [rad] if no active union or intersection, just add condition
		if(!m_pCurrentActive)
		{
			m_vecClassEntries.push_back(new ClassCondition(refOn, refValue, refOperation));
		}
		else
		{
			// [rad] delegate it to open intersection or union..
			m_pCurrentActive->AddCondition(refOn, refValue, refOperation);
		}
	}



	//--
	// [rad] Intersection is not a leaf, since it can have children
	bool ClassIntersection::VIsLeaf()
	{
		return(false);
	}




	//--
	// [rad] to satisfy an intersection, we must satisfy all things inside it
	bool ClassIntersection::VSatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->SatisfyDescriptor(pMolecule, refAux))
			{
				return(false);
			}
		}

		return(true);
	}
	
	

	//--
	// [rad] to satisfy an intersection, we must satisfy all things inside it
	bool ClassIntersection::VSatisfy(OpenBabel::OBMol* pMolecule)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pMolecule))
			{
				return(false);
			}
		}

		return(true);
	}



	//--
	// [rad] to satisfy an intersection, we must satisfy all things inside it
	bool ClassIntersection::VSatisfy(OpenBabel::OBAtom* pAtom)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pAtom))
			{
				return(false);
			}
		}

		return(true);
	}



	//--
	// [rad] to satisfy an intersection, we must satisfy all things inside it
	bool ClassIntersection::VSatisfy(OpenBabel::OBBond* pBond)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pBond))
			{
				return(false);
			}
		}

		return(true);
	}


	//--
	// [rad] to satisfy an intersection, we must satisfy all things inside it
	bool ClassIntersection::VSatisfy(OpenBabel::OBRing* pRing)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pRing))
			{
				return(false);
			}
		}

		return(true);
	}
}