#include "RelationDescriptor.h"

#include "RelationMolecule.h"
#include "RelationAtom.h"
#include "RelationBond.h"
#include "RelationRing.h"

#include "Class.h"
#include "DataProperty.h"
#include "ObjectProperty.h"

namespace Owl
{

    //--
    void RelationDescriptor::AddRelationMolecule(ObjectProperty* pProperty, RelationMolecule* pRel)
    {
        m_vecSatisfiedObjectPropertiesMolecule.push_back(std::pair<ObjectProperty*, RelationMolecule*>(pProperty, pRel));
    }
    
    
    
    
    //--
    void RelationDescriptor::AddRelationAtom(ObjectProperty* pProperty, RelationAtom* pRel)
    {
        m_vecSatisfiedObjectPropertiesAtom.push_back(std::pair<ObjectProperty*, RelationAtom*>(pProperty, pRel));
    }
    
    
    
    
    //--
    void RelationDescriptor::AddRelationBond(ObjectProperty* pProperty, RelationBond* pRel)
    {
        m_vecSatisfiedObjectPropertiesBond.push_back(std::pair<ObjectProperty*, RelationBond*>(pProperty, pRel));
    }
    
    
    
    
    //--
    void RelationDescriptor::AddRelationRing(ObjectProperty* pProperty, RelationRing* pRel)
    {
        m_vecSatisfiedObjectPropertiesRing.push_back(std::pair<ObjectProperty*, RelationRing*>(pProperty, pRel));
    }




    //--
    void RelationDescriptor::Clear()
    {
        m_vecSatisfiedClasses.clear();
        m_vecSatisfiedDataProperties.clear();

        m_vecSatisfiedObjectPropertiesMolecule.clear();
        m_vecSatisfiedObjectPropertiesAtom.clear();
        m_vecSatisfiedObjectPropertiesBond.clear();
        m_vecSatisfiedObjectPropertiesRing.clear();
    }
    
    
    
    
    //--
    void RelationDescriptor::AddClass(Class* pClass)
    {
        m_vecSatisfiedClasses.push_back(pClass);
    }
    
    
    
    
    
    //--
    void RelationDescriptor::AddData(DataProperty* pProperty, const std::string& refString)
    {
        m_vecSatisfiedDataProperties.push_back(std::pair<DataProperty*, std::string>(pProperty, refString));
    }


	// [rad] generate path and ident for descriptor
    void RelationDescriptor::GetPathAndIdentDescriptor(std::string& refPath, std::string& refIdent)
    {
        refPath = m_vecSatisfiedClasses.front()->GetPrefix() + ":" + m_vecSatisfiedClasses.front()->GetName();
        refIdent = m_vecSatisfiedClasses.front()->GetName() + "_" + std::string(m_pMol->GetTitle());
    }




	//--
    bool RelationDescriptor::IsTrivial()
    {
		// [rad] trivial means no object/data properties and only 1 class

		if(1 == m_vecSatisfiedClasses.size() && !m_vecSatisfiedDataProperties.size() &&
					!m_vecSatisfiedObjectPropertiesMolecule.size() && !m_vecSatisfiedObjectPropertiesAtom.size() &&
					!m_vecSatisfiedObjectPropertiesBond.size() && !m_vecSatisfiedObjectPropertiesRing.size())
		{
			return(true);
		}

		return(false);
    }    



	//--
    bool RelationDescriptor::HasExtraClasses()
    {
        // [rad] do we have more than 1 satisfied class?
        return(m_vecSatisfiedClasses.size() > 1);
    }



	// [rad] generate additional class info
    void RelationDescriptor::GetClassPath(int iPosition, std::string& refIdent)
    {
        refIdent = m_vecSatisfiedClasses[iPosition]->GetPrefix() + ":" + m_vecSatisfiedClasses[iPosition]->GetName();
    }



	//--
    bool RelationDescriptor::HasDataProperties()
    {
        // [rad] do we have data properties?
        return(m_vecSatisfiedDataProperties.size() > 0);
    }
    
    
    //--
    bool RelationDescriptor::HasObjectProperties()
    {
        // [rad] do we have object properties?
        return(m_vecSatisfiedObjectPropertiesMolecule.size() || m_vecSatisfiedObjectPropertiesAtom.size() ||
                m_vecSatisfiedObjectPropertiesBond.size() || m_vecSatisfiedObjectPropertiesRing.size());
    }    
    
    
    
    //--
    // [rad] generate data property info
    void RelationDescriptor::GetDataProperty(int iPosition, std::string& refPath, std::string& refType, std::string& refValue)
    {
        refPath = m_vecSatisfiedDataProperties[iPosition].first->GetPrefix() + ":" +
								m_vecSatisfiedDataProperties[iPosition].first->GetName();

        refType = m_vecSatisfiedDataProperties[iPosition].first->GetType();
        refValue = m_vecSatisfiedDataProperties[iPosition].second;			
    }



    // [rad] generate path and ident for object property pointing to molecule
    void RelationDescriptor::GetObjectPropertyPathAndIdentMolecule(int iPosition, 
                                                                const std::string& refMoleculeId, 
                                                                std::string& refPath, 
                                                                std::string& refIdent)
    {
				refPath = m_vecSatisfiedObjectPropertiesMolecule[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesMolecule[iPosition].first->GetName();
				
				std::stringstream ssConv;

				ssConv << m_vecSatisfiedObjectPropertiesMolecule[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId;

				refIdent = "";
				ssConv >> refIdent;
    }
    


	/*
    //--
        
    
    
        
    
    
    
    
    
    
        
    
    
     //--
    // [rad] generate path and ident for object property pointing to atom
    void RelationDescriptor::GetObjectPropertyPathAndIdentAtom(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent)
    {
				refPath = m_vecSatisfiedObjectPropertiesAtom[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesAtom[iPosition].first->GetName();
				
				std::stringstream ssConv;

				ssConv << m_vecSatisfiedObjectPropertiesAtom[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId
					<< "_" << m_vecSatisfiedObjectPropertiesAtom[iPosition].second->m_pAtom->GetIdx();

				refIdent = "";
				ssConv >> refIdent;
    }





    // [rad] generate path and ident for bond
    void RelationDescriptor::GetObjectPropertyPathAndIdentBond(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent)
    {
				refPath = m_vecSatisfiedObjectPropertiesBond[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesBond[iPosition].first->GetName();
				
				std::stringstream ssConv;
				
				OpenBabel::OBAtom* pAtomStart = m_vecSatisfiedObjectPropertiesBond[iPosition].second->m_pBond->GetBeginAtom();
				OpenBabel::OBAtom* pAtomEnd = m_vecSatisfiedObjectPropertiesBond[iPosition].second->m_pBond->GetEndAtom();

				ssConv << m_vecSatisfiedObjectPropertiesBond[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId << "_" <<
					pAtomStart->GetIdx() << "_"	<< pAtomEnd->GetIdx();

				refIdent = "";
				ssConv >> refIdent;
    }





    // [rad] generate path and ident for ring
    void RelationDescriptor::GetObjectPropertyPathAndIdentRing(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent)
    {
				refPath = m_vecSatisfiedObjectPropertiesRing[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesRing[iPosition].first->GetName();
				
				std::stringstream ssConv;

				OpenBabel::OBRing* pRing = m_vecSatisfiedObjectPropertiesRing[iPosition].second->m_pRing;

				ssConv << m_vecSatisfiedObjectPropertiesRing[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId;

				std::vector<int>::iterator iter_path = pRing->_path.begin();
				while(iter_path != pRing->_path.end())
				{
					ssConv << "_" << (*iter_path);					

					iter_path++;
				}

				refIdent = "";
				ssConv >> refIdent;
    }
 
	*/
}
