#include "ObjectPropertyUnion.h"
#include "ObjectPropertyCondition.h"
#include "ObjectPropertyIntersection.h"
#include "RelationBond.h"
#include "RelationRing.h"
#include "RelationMolecule.h"
#include "RelationAtom.h"
#include "RelationDescriptor.h"


namespace Owl
{
	//--
	ObjectPropertyIntersection::ObjectPropertyIntersection() : m_pCurrentActive(NULL)
	{
	}



	//--
	const std::string&	ObjectPropertyIntersection::VGetDescription() const
	{
		static std::string sDescription = "Intersection";
		return(sDescription);
	}



    //--
    ObjectPropertyIntersection::~ObjectPropertyIntersection()
    {
        std::vector<IObjectPropertyEntry*>::iterator iter_prop_entries = m_vecClassEntries.begin();
		while(m_vecClassEntries.end() != iter_prop_entries)
		{
			delete(*iter_prop_entries);
			
			// [rad] to next entry
			iter_prop_entries++;
		}
	
		m_vecClassEntries.clear();
    
    }
    
    
	//--
	void ObjectPropertyIntersection::VUnionStart()
	{
		// [rad] new union encountered

		// [rad] if we don't have active union/intersection - create it
		if(!m_pCurrentActive)
		{
			m_pCurrentActive = new ObjectPropertyUnion();
            m_vecClassEntries.push_back(m_pCurrentActive);
		}
		else
		{
			// [rad] delegate to open union/intersection
			m_pCurrentActive->UnionStart();
		}
	}



	//--
	bool ObjectPropertyIntersection::VUnionEnd()
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
	void ObjectPropertyIntersection::VIntersectionStart()
	{
		// [rad] if we don't have active union/intersection - create it
		if(!m_pCurrentActive)
		{
			m_pCurrentActive = new ObjectPropertyIntersection();
            m_vecClassEntries.push_back(m_pCurrentActive);
		}
		else
		{
			// [rad] delegate to open union/intersection
			m_pCurrentActive->IntersectionStart();
		}
	}



	//--
	bool ObjectPropertyIntersection::VIntersectionEnd()
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
	void ObjectPropertyIntersection::VAddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
	{
		// [rad] if no active union or intersection, just add condition
		if(!m_pCurrentActive)
		{
            m_vecClassEntries.push_back(new ObjectPropertyCondition(refOn, refValue, refOperation));
		}
		else
		{
			// [rad] delegate it to open intersection or union..
			m_pCurrentActive->AddCondition(refOn, refValue, refOperation);
		}
	}




	//--
	// [rad] Union is not a leaf, since it can have children
	bool ObjectPropertyIntersection::VIsLeaf()
	{
		return(false);
	}




	//--
    bool ObjectPropertyIntersection::VSatisfy(RelationDescriptor* pRelDescriptor)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pRelDescriptor))
			{
				return(false);
			}
		}

		return(true);
	}



	//--
    bool ObjectPropertyIntersection::VSatisfy(RelationMolecule* pRelMolecule)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pRelMolecule))
			{
				return(false);
			}
		}

		return(true);
	}


	//--
    bool ObjectPropertyIntersection::VSatisfy(RelationAtom* pRelAtom)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pRelAtom))
			{
				return(false);
			}
		}

		return(true);
	}


    //--
    bool ObjectPropertyIntersection::VSatisfy(RelationBond* pRelBond)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pRelBond))
			{
				return(false);
			}
		}

		return(true);
	}


	//--
    bool ObjectPropertyIntersection::VSatisfy(RelationRing* pRelRing)
	{
		for(int i = 0; i < m_vecClassEntries.size(); i++)
		{
			if(!m_vecClassEntries[i]->Satisfy(pRelRing))
			{
				return(false);
			}
		}

		return(true);
	}
}