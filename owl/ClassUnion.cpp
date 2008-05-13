#include "ClassUnion.h"
#include "ClassCondition.h"
#include "ClassIntersection.h"

namespace Owl
{
	//--
	ClassUnion::ClassUnion() : m_pCurrentActive(NULL)
	{
	}



	//--
	const std::string&	ClassUnion::VGetDescription() const
	{
		static std::string sDescription = "Union";
		return(sDescription);
	}



	//--
	ClassUnion::~ClassUnion()
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
	void ClassUnion::VUnionStart()
	{
		// [rad] new union encountered

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
	bool ClassUnion::VUnionEnd()
	{
		// [rad] if we don't have active union/intersection - then we must return to parent and close this union
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
	void ClassUnion::VIntersectionStart()
	{
		// [rad] if we don't have active union/intersection - create it
		if(!m_pCurrentActive)
		{
			m_pCurrentActive = new ClassIntersection();
			//m_vecClassEntries.push_back(Ptr_ClassEntries(m_pCurrentActive));
			m_vecClassEntries.push_back(m_pCurrentActive);
		}
		else
		{
			// [rad] delegate to open union/intersection
			m_pCurrentActive->IntersectionStart();
		}
	}



	//--
	bool ClassUnion::VIntersectionEnd()
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
	void ClassUnion::VAddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
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
	// [rad] Union is not a leaf, since it can have children
	bool ClassUnion::VIsLeaf()
	{
		return(false);
	}



	//--
	// [rad] to satisfy a union, we must satisfy only one thing inside a union
	bool ClassUnion::VSatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux)
	{
		// [rad] if empty - satisfied, alright
		if(!m_vecClassEntries.size()) return(true);
	
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(m_vecClassEntries[i]->SatisfyDescriptor(pMolecule, refAux))
			{
				return(true);
			}
		}

		return(false);
	}
	
	
	//--
	// [rad] to satisfy a union, we must satisfy only one thing inside a union
	bool ClassUnion::VSatisfy(OpenBabel::OBMol* pMolecule)
	{
		// [rad] if empty - satisfied
		if(!m_vecClassEntries.size()) return(true);

		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(m_vecClassEntries[i]->Satisfy(pMolecule))
			{
				return(true);
			}
		}

		return(false);
	}



	//--
	// [rad] to satisfy a union, we must satisfy only one thing inside a union
	bool ClassUnion::VSatisfy(OpenBabel::OBAtom* pAtom)
	{
		// [rad] if empty - satisfied
		if(!m_vecClassEntries.size()) return(true);

		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(m_vecClassEntries[i]->Satisfy(pAtom))
			{
				return(true);
			}
		}

		return(false);
	}



	//--
	// [rad] to satisfy a union, we must satisfy only one thing inside a union
	bool ClassUnion::VSatisfy(OpenBabel::OBBond* pBond)
	{
		// [rad] if empty - satisfied
		if(!m_vecClassEntries.size()) return(true);

		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(m_vecClassEntries[i]->Satisfy(pBond))
			{
				return(true);
			}
		}

		return(false);
	}



	//--
	// [rad] to satisfy a union, we must satisfy only one thing inside a union
	bool ClassUnion::VSatisfy(OpenBabel::OBRing* pRing)
	{
		// [rad] if empty - satisfied
		if(!m_vecClassEntries.size()) return(true);

		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(m_vecClassEntries[i]->Satisfy(pRing))
			{
				return(true);
			}
		}

		return(false);
	}
}