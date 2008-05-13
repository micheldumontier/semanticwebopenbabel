#include "DataPropertyResource.h"
#include "RelationBond.h"
#include "RelationAtom.h"
#include "RelationMolecule.h"
#include "RelationRing.h"
#include "RelationDescriptor.h"

namespace Owl
{
	//--
	DataPropertyResource::DataPropertyResource() : m_sFrom(""), m_sTo(""), m_pCachedDataProperty(NULL)
	{

	}



	//--
	DataPropertyResource::DataPropertyResource(const std::string& refFrom, const std::string& refTo) :
	m_sFrom(refFrom), m_sTo(refTo), m_pCachedDataProperty(NULL)
	{

	}



	//--
	DataPropertyResource::~DataPropertyResource()
	{
		std::vector<DataProperty*>::iterator iter_dataprop_entries = m_vecDataProperties.begin();
		while(m_vecDataProperties.end() != iter_dataprop_entries)
		{
			delete(*iter_dataprop_entries);
			
			// [rad] to next entry
			iter_dataprop_entries++;
		}
	
		m_vecDataProperties.clear();
	}

	
	
	
	//--
	const std::string& DataPropertyResource::GetFrom() const
	{
		return(m_sFrom);
	}



	//--
	const std::string& DataPropertyResource::GetTo() const
	{
		return(m_sTo);
	}



	//--
	void DataPropertyResource::AddDataProperty(const std::string& refPrefix, const std::string& refName, const std::string& refType)
	{
		m_pCachedDataProperty = new DataProperty(refPrefix, refName, refType);
		m_vecDataProperties.push_back(m_pCachedDataProperty);
	}


	//--
	void DataPropertyResource::AddOption(const std::string& refPrefix, const std::string& refName)
	{
		if(m_pCachedDataProperty)
		{
			m_pCachedDataProperty->AddOption(refPrefix, refName);
		}
	}



	//--
	void DataPropertyResource::RetrieveProperty(std::set<std::string>& refDataProperties)
	{
		for(int i = 0; i < m_vecDataProperties.size(); i++) 
		{
			m_vecDataProperties[i]->RetrieveProperty(refDataProperties);
		}
	}



	//--
	void DataPropertyResource::Print(std::ostream* pOutputStream)
	{
		for(int i = 0; i < m_vecDataProperties.size(); i++) 
		{
			m_vecDataProperties[i]->Print(pOutputStream);
		}
	}




	//--
	// [rad] do we only have default property?
	bool DataPropertyResource::HasSingleDefault() const
	{
		if(!m_vecDataProperties.size()) return(false);

		if(1 == m_vecDataProperties.size())
		{
			return(m_vecDataProperties.front()->IsDefault());
		}

		return(false);
	}




	//--
	// [rad] do we have default and non default properties?
	bool DataPropertyResource::HasMultipleDefault() const
	{
		bool bHaveDefault = false;
		bool bHaveNonDefault = false;

		for(int i = 0 ; i < m_vecDataProperties.size(); i++)
		{
			if(m_vecDataProperties[i]->IsDefault())
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
	bool DataPropertyResource::Satisfy(OpenBabel::OBMol* pMolecule,
										RelationDescriptor* pRel,
										GenerateOptions& refOptions)
	{
		
		if(m_sFrom.compare("DESCRIPTOR")) return(false);
		
		// [rad] check if this data property resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();
		
		std::string sValue;
		
		for(int i = 0 ; i < m_vecDataProperties.size(); i++)
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecDataProperties[i]->IsDefault()) continue;

			if(m_vecDataProperties[i]->SatisfyDirectDescriptor(pMolecule, m_sTo, pRel->m_vecSatisfiedClasses, sValue))
			{
				 pRel->AddData(m_vecDataProperties[i], sValue);

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}

		}

		
		return(true);
	}


    //--
    bool DataPropertyResource::Satisfy(OpenBabel::OBMol* pMolecule,
                                        RelationMolecule* pRel,
                                        GenerateOptions& refOptions)
    {
        if(m_sFrom.compare("MOLECULE")) return(false);

		// [rad] check if this data property resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		std::string sValue;

		for(int i = 0 ; i < m_vecDataProperties.size(); i++)
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecDataProperties[i]->IsDefault()) continue;

			if(m_vecDataProperties[i]->Satisfy(pMolecule, m_sTo, pRel->m_vecSatisfiedClasses, sValue))
			{
			    pRel->AddData(m_vecDataProperties[i], sValue);

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}

		}

		return(true);

    }
    
    
    


    //--
    bool DataPropertyResource::Satisfy(OpenBabel::OBAtom* pAtom,
                                            RelationAtom* pRel,
                                            GenerateOptions& refOptions)
    {
        if(m_sFrom.compare("ATOM")) return(false);

		// [rad] check if this data property resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		std::string sValue;

		for(int i = 0 ; i < m_vecDataProperties.size(); i++)
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecDataProperties[i]->IsDefault()) continue;

			if(m_vecDataProperties[i]->Satisfy(pAtom, m_sTo, pRel->m_vecSatisfiedClasses, sValue))
			{
				pRel->AddData(m_vecDataProperties[i], sValue);

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}

		}

		return(true);

    }
        
                                                            
                                                                                                                                                            
        
    

    //--
    bool DataPropertyResource::Satisfy(OpenBabel::OBRing* pRing,
                                            RelationRing* pRel,
                                            GenerateOptions& refOptions)
    {
        if(m_sFrom.compare("RING")) return(false);

		// [rad] check if this data property resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		std::string sValue;

		for(int i = 0 ; i < m_vecDataProperties.size(); i++)
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecDataProperties[i]->IsDefault()) continue;

			if(m_vecDataProperties[i]->Satisfy(pRing, m_sTo, pRel->m_vecSatisfiedClasses, sValue))
			{
				pRel->AddData(m_vecDataProperties[i], sValue);

				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}

		}

		return(true);
    
    }
            
                                                            
        
    

    //--
    bool DataPropertyResource::Satisfy(OpenBabel::OBBond* pBond,
                                        RelationBond* pRel,
                                        GenerateOptions& refOptions)
    {
        
        if(m_sFrom.compare("BOND")) return(false);

		// [rad] check if this data property resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		std::string sValue;

		for(int i = 0 ; i < m_vecDataProperties.size(); i++)
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecDataProperties[i]->IsDefault()) continue;

			if(m_vecDataProperties[i]->Satisfy(pBond, m_sTo, pRel->m_vecSatisfiedClasses, sValue))
			{
				pRel->AddData(m_vecDataProperties[i], sValue);
				
				// [rad] we are enforcing single type
				if(refOptions.m_bGenerateForceSingleType)
				{
					return(true);
				}
			}

		}

		return(true);

    }
        
                                                            
}