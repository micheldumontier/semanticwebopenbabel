#include "ObjectPropertyResource.h"
#include "Relation.h"
#include "RelationMolecule.h"
#include "RelationRing.h"
#include "RelationBond.h"
#include "RelationAtom.h"
#include "RelationDescriptor.h"

namespace Owl
{
	//--
	ObjectPropertyResource::ObjectPropertyResource() : m_sTo(""), m_sFrom(""), m_sConstraint(""), m_pCachedObjectProperty(NULL)
	{

	}




	//--
	ObjectPropertyResource::ObjectPropertyResource(const std::string& refFrom, const std::string& refTo, const std::string& refConstraint) 
		: m_sFrom(refFrom), m_sTo(refTo), m_sConstraint(refConstraint), m_pCachedObjectProperty(NULL)
	{

	}
    
    
    //--
    ObjectPropertyResource::~ObjectPropertyResource()
    {
        std::vector<ObjectProperty*>::iterator iter_prop_entries = m_vecObjectProperties.begin();
		while(m_vecObjectProperties.end() != iter_prop_entries)
		{
			delete(*iter_prop_entries);
			
			// [rad] to next entry
			iter_prop_entries++;
		}
	
		m_vecObjectProperties.clear();

    }




	//--
	const std::string& ObjectPropertyResource::GetTo() const
	{
		return(m_sTo);
	}




	//--
	const std::string& ObjectPropertyResource::GetFrom() const
	{
		return(m_sFrom);
	}



	//--
	const std::string& ObjectPropertyResource::GetConstraint() const
	{
		return(m_sConstraint);
	}



	//--
	void ObjectPropertyResource::AddObjectProperty(const std::string& refPrefix, const std::string& refName, const std::string& refAuxiliary)
	{
		m_pCachedObjectProperty = new ObjectProperty(refPrefix, refName, refAuxiliary);
        m_vecObjectProperties.push_back(m_pCachedObjectProperty);
	}




	//--
	void ObjectPropertyResource::AddDomain(const std::string& refPrefix, const std::string& refName)
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->AddDomain(refPrefix, refName);
		}
	}




	//--
	void ObjectPropertyResource::AddRange(const std::string& refPrefix, const std::string& refName)
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->AddRange(refPrefix, refName);
		}
	}




	//--
	void ObjectPropertyResource::AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->AddCondition(refOn, refValue, refOperation);
		}
	}




	//--
	void ObjectPropertyResource::IntersectionEnd()
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->IntersectionEnd();
		}
	}




	//--
	void ObjectPropertyResource::IntersectionStart()
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->IntersectionStart();
		}
	}




	//--
	void ObjectPropertyResource::UnionStart()
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->UnionStart();
		}
	}




	//--
	void ObjectPropertyResource::UnionEnd()
	{
		if(m_pCachedObjectProperty)
		{
			m_pCachedObjectProperty->UnionEnd();
		}
	}




	//--
	void ObjectPropertyResource::Print(std::ostream* pOutputStream)
	{
		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			m_vecObjectProperties[i]->Print(pOutputStream);
		}
	}



	//--
	void ObjectPropertyResource::RetrieveProperty(std::set<std::string>& refObjectProperties)
	{
		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			m_vecObjectProperties[i]->RetrieveProperty(refObjectProperties);
		}
	}



	//--
	bool ObjectPropertyResource::HasMultipleDefault() const
	{
		bool bHaveDefault = false;
		bool bHaveNonDefault = false;

		for(int i = 0 ; i < m_vecObjectProperties.size(); i++)
		{
			if(m_vecObjectProperties[i]->IsDefault())
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
    bool ObjectPropertyResource::Satisfy(RelationMolecule* pRelMolecule, 
                                           RelationDescriptor* pRelDescriptor,
                                           GenerateOptions& refOptions,
                                           bool bForward)
    {
    
        // [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("MOLECULE") || m_sTo.compare("DESCRIPTOR")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("DESCRIPTOR") || m_sTo.compare("MOLECULE")) 
			{
				return(false);
			}
		}	
		
		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;

			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelMolecule, pRelDescriptor, bForward))
			{	
				if(bForward)
				{       
                    pRelMolecule->AddRelationDescriptor(m_vecObjectProperties[i], pRelDescriptor);
                }
				else
				{
                    pRelDescriptor->AddRelationMolecule(m_vecObjectProperties[i], pRelMolecule);
				}

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
    bool ObjectPropertyResource::Satisfy(RelationMolecule* pRelMolecule, 
                                           RelationAtom* pRelAtom,
                                           GenerateOptions& refOptions,
                                           bool bForward)
    {
    
        // [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("MOLECULE") || m_sTo.compare("ATOM")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("ATOM") || m_sTo.compare("MOLECULE")) 
			{
				return(false);
			}
		}	

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();

		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;

			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelMolecule, pRelAtom, bForward))
			{	
				if(bForward)
				{       
                    pRelMolecule->AddRelationAtom(m_vecObjectProperties[i], pRelAtom);
                }
				else
				{
                    pRelAtom->AddRelationMolecule(m_vecObjectProperties[i], pRelMolecule);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationMolecule* pRelMolecule, 
                                            RelationBond* pRelBond, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{        
		// [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("MOLECULE") || m_sTo.compare("BOND")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("BOND") || m_sTo.compare("MOLECULE")) 
			{
				return(false);
			}
		}	

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelMolecule, pRelBond, bForward))
			{	
				if(bForward)
				{
                    pRelMolecule->AddRelationBond(m_vecObjectProperties[i], pRelBond);
				}
				else
				{
                    pRelBond->AddRelationMolecule(m_vecObjectProperties[i], pRelMolecule);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationMolecule* pRelMolecule, 
                                            RelationRing* pRelRing, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{
        // [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("MOLECULE") || m_sTo.compare("RING")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("RING") || m_sTo.compare("MOLECULE")) 
			{
				return(false);
			}
		}	

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelMolecule, pRelRing, bForward))
			{	
				if(bForward)
				{
                    pRelMolecule->AddRelationRing(m_vecObjectProperties[i], pRelRing);
				}
				else
				{
                    pRelRing->AddRelationMolecule(m_vecObjectProperties[i], pRelMolecule);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationAtom* pRelAtom, 
                                            RelationBond* pRelBond, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{
        // [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("ATOM") || m_sTo.compare("BOND")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("BOND") || m_sTo.compare("ATOM")) 
			{
				return(false);
			}
		}	

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;

			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelAtom, pRelBond, bForward))
			{	
				if(bForward)
				{
                    pRelAtom->AddRelationBond(m_vecObjectProperties[i], pRelBond);
				}
				else
				{
                    pRelBond->AddRelationAtom(m_vecObjectProperties[i], pRelAtom);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationAtom* pRelAtom, 
                                            RelationRing* pRelRing, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{
        // [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("ATOM") || m_sTo.compare("RING")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("RING") || m_sTo.compare("ATOM")) 
			{
				return(false);
			}
		}	

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelAtom, pRelRing, bForward))
			{	
				if(bForward)
				{
                    pRelAtom->AddRelationRing(m_vecObjectProperties[i], pRelRing);
				}
				else
				{
                    pRelRing->AddRelationAtom(m_vecObjectProperties[i], pRelAtom);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationBond* pRelBond, 
                                            RelationRing* pRelRing, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
    {
        
        // [rad] check if range/domain match
		if(bForward)
		{
			if(m_sFrom.compare("BOND") || m_sTo.compare("RING")) 
			{
				return(false);
			}
		}
		else
		{
			if(m_sFrom.compare("RING") || m_sTo.compare("BOND")) 
			{
				return(false);
			}
		}	

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
		
			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelBond, pRelRing, bForward))
			{	
				// [rad] if we are ignoring multiple defaults
				if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


				if(bForward)
				{
                    pRelBond->AddRelationRing(m_vecObjectProperties[i], pRelRing);
				}
				else
				{
                    pRelRing->AddRelationBond(m_vecObjectProperties[i], pRelBond);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationMolecule* pRelMolecule, 
                                            RelationMolecule* pRelMolecule2, 
                                            GenerateOptions& refOptions,  
                                            bool bForward)
	{
    
        
		if(m_sFrom.compare("MOLECULE") || m_sTo.compare("MOLECULE")) 
		{
			return(false);
		}

		// [rad] check constraint type
		if(!m_sConstraint.compare("same"))
		{
			// [rad] must be same individual
			if(pRelMolecule->m_pMol != pRelMolecule2->m_pMol)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("different"))
		{
			// [rad] must be different individuals
			if(pRelMolecule->m_pMol == pRelMolecule2->m_pMol)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("connected"))
		{
			// [rad] connected behavior is undefined
		}

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelMolecule, pRelMolecule2, bForward))
			{	
				if(bForward)
				{
                    pRelMolecule->AddRelationMolecule(m_vecObjectProperties[i], pRelMolecule2);
				}
				else
				{
                    pRelMolecule2->AddRelationMolecule(m_vecObjectProperties[i], pRelMolecule);
                }

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
	bool ObjectPropertyResource::Satisfy(RelationAtom* pRelAtom, 
                                            RelationAtom* pRelAtom2, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{        
		if(m_sFrom.compare("ATOM") || m_sTo.compare("ATOM")) 
		{
			return(false);
		}

		bool bConnected = false;

		// [rad] check constraint type
		if(!m_sConstraint.compare("same"))
		{
			// [rad] must be same individual
			if(pRelAtom->m_pAtom != pRelAtom2->m_pAtom)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("different"))
		{
			// [rad] must be different individuals
			if(pRelAtom->m_pAtom == pRelAtom2->m_pAtom)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("connected"))
		{
			// [rad] we need to bypass satisfy routine
			bConnected = true;
		}


		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			if(!bConnected)
			{
				// [rad] delegate..
				if(m_vecObjectProperties[i]->Satisfy(pRelAtom, pRelAtom2, bForward))
				{	
					if(bForward)
					{
                        pRelAtom->AddRelationAtom(m_vecObjectProperties[i], pRelAtom2);
					}
					else
					{
                        pRelAtom2->AddRelationAtom(m_vecObjectProperties[i], pRelAtom);
					}

					// [rad] we are enforcing single type
					if(refOptions.m_bGenerateForceSingleType)
					{
						return(true);
					}
				}
			}
			else
			{
				// [rad] delegate to special connected 
				if(m_vecObjectProperties[i]->SatisfyConnected(pRelAtom, pRelAtom2, bForward))
				{	
					if(bForward)
					{
                        pRelAtom->AddRelationAtom(m_vecObjectProperties[i], pRelAtom2);
					}
					else
					{
                        pRelAtom2->AddRelationAtom(m_vecObjectProperties[i], pRelAtom);
					}

					// [rad] we are enforcing single type
					if(refOptions.m_bGenerateForceSingleType)
					{
						return(true);
					}
				}
			}
		}

		return(true);
	}





	//--
	bool ObjectPropertyResource::Satisfy(RelationBond* pRelBond, 
                                            RelationBond* pRelBond2, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{
        
		if(m_sFrom.compare("BOND") || m_sTo.compare("BOND")) 
		{
			return(false);
		}

		// [rad] check constraint type
		if(!m_sConstraint.compare("same"))
		{
			// [rad] must be same individual
			if(pRelBond->m_pBond != pRelBond2->m_pBond)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("different"))
		{
			// [rad] must be different individuals
			if(pRelBond->m_pBond == pRelBond2->m_pBond)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("different"))
		{
			// [rad] connected behavior is undefined
		}

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelBond, pRelBond2, bForward))
			{	
				if(bForward)
				{
                    pRelBond->AddRelationBond(m_vecObjectProperties[i], pRelBond2);
				}
				else
				{
                    pRelBond2->AddRelationBond(m_vecObjectProperties[i], pRelBond);
				}

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
	bool ObjectPropertyResource::Satisfy(RelationRing* pRelRing, 
                                            RelationRing* pRelRing2, 
                                            GenerateOptions& refOptions, 
                                            bool bForward)
	{
		if(m_sFrom.compare("RING") || m_sTo.compare("RING")) 
		{
			return(false);
		}

		// [rad] check constraint type
		if(!m_sConstraint.compare("same"))
		{
			// [rad] must be same individual
			if(pRelRing->m_pRing != pRelRing2->m_pRing)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("different"))
		{
			// [rad] must be different individuals
			if(pRelRing->m_pRing == pRelRing2->m_pRing)
			{
				return(false);
			}
		}
		else if(!m_sConstraint.compare("different"))
		{
			// [rad] connected behavior is undefined
		}

		// [rad] check if this class resource has multiple default
		bool bMultipleDefault = HasMultipleDefault();


		for(int i = 0; i < m_vecObjectProperties.size(); i++) 
		{
			// [rad] if we are ignoring multiple defaults
			if(!refOptions.m_bGenerateUseMultipleDefault && bMultipleDefault && m_vecObjectProperties[i]->IsDefault()) continue;


			// [rad] delegate..
			if(m_vecObjectProperties[i]->Satisfy(pRelRing, pRelRing2, bForward))
			{	
				if(bForward)
				{
                    pRelRing->AddRelationRing(m_vecObjectProperties[i], pRelRing2);
				}
				else
				{
                    pRelRing2->AddRelationRing(m_vecObjectProperties[i], pRelRing);
				}

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