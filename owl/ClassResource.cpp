#include "ClassResource.h"

#include "RelationRing.h"
#include "RelationAtom.h"
#include "RelationBond.h"
#include "RelationMolecule.h"
#include "RelationDescriptor.h"

namespace Owl
{
	//--
	ClassResource::ClassResource() : m_sName(""), m_sAuxiliary(""), m_pCachedClass(NULL)
	{

	}



	//--
	ClassResource::ClassResource(const std::string& refName) : m_sName(refName), 
		m_sAuxiliary(""), m_pCachedClass(NULL)
	{

	}
	
	
	//--
	ClassResource::ClassResource(const std::string& refName, const std::string& refAuxiliary) 
		: m_sName(refName), m_sAuxiliary(refAuxiliary), m_pCachedClass(NULL)
	{

	}



	//--
	ClassResource::~ClassResource()
	{
		std::vector<Class*>::iterator iter_class_entries = m_vecClasses.begin();
		while(m_vecClasses.end() != iter_class_entries)
		{
			delete(*iter_class_entries);
			
			// [rad] to next entry
			iter_class_entries++;
		}
	
		m_vecClasses.clear();
	
	}
	
	
	
	//--
	const std::string& ClassResource::GetName() const
	{
		return(m_sName);
	}
	
	
	//--
	const std::string& ClassResource::GetAuxiliary() const
	{
		return(m_sAuxiliary);
	}




	//--
	void ClassResource::AddClass(const std::string& refPrefix, const std::string& refName)
	{
		m_pCachedClass = new Class(refPrefix, refName);
		m_vecClasses.push_back(m_pCachedClass);
	}



	//--
	void ClassResource::AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
	{
		if(m_pCachedClass)
		{
			m_pCachedClass->AddCondition(refOn, refValue, refOperation);
		}
	}



	//--
	void ClassResource::IntersectionEnd()
	{
		if(m_pCachedClass)
		{
			m_pCachedClass->IntersectionEnd();
		}
	}


	//--
	void ClassResource::IntersectionStart()
	{
		if(m_pCachedClass)
		{
			m_pCachedClass->IntersectionStart();
		}
	}



	//--
	void ClassResource::UnionStart()
	{
		if(m_pCachedClass)
		{
			m_pCachedClass->UnionStart();
		}
	}



	//--
	void ClassResource::UnionEnd()
	{
		if(m_pCachedClass)
		{
			m_pCachedClass->UnionEnd();
		}
	}



	//--
	void ClassResource::RetrieveClass(std::set<std::string>& refClasses)
	{
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			m_vecClasses[i]->RetrieveClass(refClasses);
		}
	}


	//--
	void ClassResource::Print(std::ostream* pOutputStream)
	{
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			m_vecClasses[i]->Print(pOutputStream);
		}
	}







	//--
	bool ClassResource::HasMultipleDefault() const
	{
		bool bHaveDefault = false;
		bool bHaveNonDefault = false;

		for(int i = 0 ; i < m_vecClasses.size(); i++)
		{
			if(m_vecClasses[i]->IsDefault())
			{
				// [rad] already have a default
				//if(bHaveDefault) return(true);

				bHaveDefault = true;
			}
			else
			{
				bHaveNonDefault = true;
			}
		}

		if(bHaveDefault && bHaveNonDefault) return(true);

		return(false);
	}




	//--
	bool ClassResource::Satisfy(OpenBabel::OBMol* pMolecule,
									RelationDescriptor* pRel,
									GenerateOptions& refOptions)
	{
		if(m_sName.compare("DESCRIPTOR")) return(false);
		
		bool bSatisfied = false;
		
		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();
		
		// [rad] otherwise go through all classes and see which ones "fit"
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecClasses[i]->IsDefault()) continue;
			
			if(m_vecClasses[i]->SatisfyDescriptor(pMolecule, m_sAuxiliary))
			{
				pRel->AddClass(m_vecClasses[i]);
				
				bSatisfied = true;
				
				// [rad] We are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}
		}
	
		return(bSatisfied);
	}
	
	

    //--
    bool ClassResource::Satisfy(OpenBabel::OBMol* pMolecule,
                                    RelationMolecule* pRel,
                                    GenerateOptions& refOptions)
    {
        if(m_sName.compare("MOLECULE")) return(false);

		bool bSatisfied = false;

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		// [rad] otherwise go through all classes and see which ones "fit"
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecClasses[i]->IsDefault()) continue;

			if(m_vecClasses[i]->Satisfy(pMolecule))
			{
                pRel->AddClass(m_vecClasses[i]);

				bSatisfied = true;

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}
		}

		return(bSatisfied);
    }





    //--
    bool ClassResource::Satisfy(OpenBabel::OBAtom* pAtom,
                                    RelationAtom* pRel,
                                    GenerateOptions& refOptions)
    {
    
        if(m_sName.compare("ATOM")) return(false);

		bool bSatisfied = false;

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		// [rad] otherwise go through all classes and see which ones "fit"
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecClasses[i]->IsDefault()) continue;

			if(m_vecClasses[i]->Satisfy(pAtom))
			{
                pRel->AddClass(m_vecClasses[i]);

				bSatisfied = true;

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}
		}

		return(bSatisfied);
    }







    //--
    bool ClassResource::Satisfy(OpenBabel::OBRing* pRing,
                                  RelationRing* pRel,
                                  GenerateOptions& refOptions)
    {
        if(m_sName.compare("RING")) return(false);

		bool bSatisfied = false;

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		// [rad] otherwise go through all classes and see which ones "fit"
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecClasses[i]->IsDefault()) continue;


			if(m_vecClasses[i]->Satisfy(pRing))
			{
                pRel->AddClass(m_vecClasses[i]);

				bSatisfied = true;

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}
		}

		return(bSatisfied);

    }
    
    
    




    //--
    bool ClassResource::Satisfy(OpenBabel::OBBond* pBond,
                                 RelationBond* pRel,
                                 GenerateOptions& refOptions)
    {
        if(m_sName.compare("BOND")) return(false);

		bool bSatisfied = false;

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		// [rad] otherwise go through all classes and see which ones "fit"
		for(int i = 0; i < m_vecClasses.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecClasses[i]->IsDefault()) continue;


			if(m_vecClasses[i]->Satisfy(pBond))
			{
                pRel->AddClass(m_vecClasses[i]);

				bSatisfied = true;

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}
		}

		return(bSatisfied);

    }
    
}