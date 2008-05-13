#include "ObjectPropertyDomain.h"
#include "RelationRing.h"
#include "RelationBond.h"
#include "RelationAtom.h"
#include "RelationMolecule.h"
#include "RelationDescriptor.h"
#include "Class.h"

namespace Owl
{
	//--
	ObjectPropertyDomain::ObjectPropertyDomain() : m_sPrefix(""), m_sName(""), m_ptrRoot(NULL)
	{
		m_ptrRoot = new ObjectPropertyIntersection();
	}



	//--
	ObjectPropertyDomain::ObjectPropertyDomain(const std::string& refPrefix, const std::string& refName) : 
        m_sPrefix(refPrefix), m_sName(refName), m_ptrRoot(NULL)
	{
		m_ptrRoot = new ObjectPropertyIntersection();
	}



    //--
    ObjectPropertyDomain::~ObjectPropertyDomain()
    {
        if(m_ptrRoot)
        {
            delete(m_ptrRoot);
            m_ptrRoot = NULL;
        }
    }



	//--
	const std::string& ObjectPropertyDomain::GetPrefix() const
	{
		return(m_sPrefix);
	}



	//--
	const std::string& ObjectPropertyDomain::GetName() const
	{
		return(m_sName);
	}




	//--
	void ObjectPropertyDomain::UnionStart()
	{
		m_ptrRoot->UnionStart();
	}




	//--
	void ObjectPropertyDomain::UnionEnd()
	{
		m_ptrRoot->UnionEnd();
	}




	//--
	void ObjectPropertyDomain::IntersectionStart()
	{
		m_ptrRoot->IntersectionStart();
	}




	//--
	void ObjectPropertyDomain::IntersectionEnd()
	{
		m_ptrRoot->IntersectionEnd();
	}



	//--
	void ObjectPropertyDomain::AddCondition(const std::string& refOn, 
                                            const std::string& refValue, 
                                            const std::string& refOperation)
	{
		m_ptrRoot->AddCondition(refOn, refValue, refOperation);
	}




	//-
	bool ObjectPropertyDomain::Satisfy(RelationDescriptor* pRelDescriptor)
	{
		bool bMatched = false;

		for(int i = 0; i < pRelDescriptor->m_vecSatisfiedClasses.size(); i++)
		{
			if(!pRelDescriptor->m_vecSatisfiedClasses[i]->GetName().compare(m_sName) &&
					!pRelDescriptor->m_vecSatisfiedClasses[i]->GetPrefix().compare(m_sPrefix))
			{
				bMatched = true;
				break;
			}
		}

		if(!bMatched) return(false);

		return(m_ptrRoot->Satisfy(pRelDescriptor));
	}
	
	
	
	//--
    bool ObjectPropertyDomain::Satisfy(RelationMolecule* pRelMolecule)
	{
		bool bMatched = false;

		for(int i = 0; i < pRelMolecule->m_vecSatisfiedClasses.size(); i++)
		{
			if(!pRelMolecule->m_vecSatisfiedClasses[i]->GetName().compare(m_sName) &&
					!pRelMolecule->m_vecSatisfiedClasses[i]->GetPrefix().compare(m_sPrefix))
			{
				bMatched = true;
				break;
			}
		}

		if(!bMatched) return(false);

		return(m_ptrRoot->Satisfy(pRelMolecule));
	}





	//--
    bool ObjectPropertyDomain::Satisfy(RelationAtom* pRelAtom)
	{
		bool bMatched = false;

		for(int i = 0; i < pRelAtom->m_vecSatisfiedClasses.size(); i++)
		{
			if(!pRelAtom->m_vecSatisfiedClasses[i]->GetName().compare(m_sName) &&
					!pRelAtom->m_vecSatisfiedClasses[i]->GetPrefix().compare(m_sPrefix))
			{
				bMatched = true;
				break;
			}
		}

		if(!bMatched) return(false);

		return(m_ptrRoot->Satisfy(pRelAtom));
	}



	
    
    //--
    bool ObjectPropertyDomain::Satisfy(RelationRing* pRelRing)
	{
		bool bMatched = false;

		for(int i = 0; i < pRelRing->m_vecSatisfiedClasses.size(); i++)
		{
			if(!pRelRing->m_vecSatisfiedClasses[i]->GetName().compare(m_sName) &&
					!pRelRing->m_vecSatisfiedClasses[i]->GetPrefix().compare(m_sPrefix))
			{
				bMatched = true;
				break;
			}
		}

		if(!bMatched) return(false);

		return(m_ptrRoot->Satisfy(pRelRing));
	}





	//--
    bool ObjectPropertyDomain::Satisfy(RelationBond* pRelBond)
	{
		bool bMatched = false;

		for(int i = 0; i < pRelBond->m_vecSatisfiedClasses.size(); i++)
		{
			if(!pRelBond->m_vecSatisfiedClasses[i]->GetName().compare(m_sName) &&
					!pRelBond->m_vecSatisfiedClasses[i]->GetPrefix().compare(m_sPrefix))
			{
				bMatched = true;
				break;
			}
		}

		if(!bMatched) return(false);

		return(m_ptrRoot->Satisfy(pRelBond));
	}

}