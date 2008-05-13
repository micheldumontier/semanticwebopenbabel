#if !defined(OWL_RELATION_H)
#define OWL_RELATION_H

#include <string>
#include <vector>
#include <sstream>

#include "Class.h"
#include "DataProperty.h"
#include "ObjectProperty.h"

#include <openbabel/mol.h>

namespace Owl
{

	template <typename T>
	class Relation
	{
        friend class ObjectProperty;
		
		public:
			
			void Clear()
			{
				m_vecSatisfiedClasses.clear();
				m_vecSatisfiedDataProperties.clear();

				m_vecSatisfiedObjectPropertiesMolecule.clear();
				m_vecSatisfiedObjectPropertiesAtom.clear();
				m_vecSatisfiedObjectPropertiesBond.clear();
				m_vecSatisfiedObjectPropertiesRing.clear();
			}


			bool IsTrivial()
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


			bool HasExtraClasses()
			{
				// [rad] do we have more than 1 satisfied class?
				return(m_vecSatisfiedClasses.size() > 1);
			}

			bool HasDataProperties()
			{
				// [rad] do we have data properties?
				return(m_vecSatisfiedDataProperties.size() > 0);
			}


			bool HasObjectProperties()
			{
				// [rad] do we have object properties?
				return(m_vecSatisfiedObjectPropertiesMolecule.size() || m_vecSatisfiedObjectPropertiesAtom.size() ||
						m_vecSatisfiedObjectPropertiesBond.size() || m_vecSatisfiedObjectPropertiesRing.size());
			}


			// [rad] generate additional class info
			void GetClassPath(int iPosition, std::string& refIdent)
			{
				refIdent = m_vecSatisfiedClasses[iPosition]->GetPrefix() + ":" +
					m_vecSatisfiedClasses[iPosition]->GetName();
			}

			// [rad] generate data property info
			void GetDataProperty(int iPosition, std::string& refPath, std::string& refType, std::string& refValue)
			{
				refPath = m_vecSatisfiedDataProperties[iPosition].first->GetPrefix() + ":" +
								m_vecSatisfiedDataProperties[iPosition].first->GetName();

				refType = m_vecSatisfiedDataProperties[iPosition].first->GetType();
				refValue = m_vecSatisfiedDataProperties[iPosition].second;			
			}





			// [rad] generate path and ident for molecule
			void GetPathAndIdentMolecule(std::string& refPath, std::string& refIdent)
			{
				refPath = m_vecSatisfiedClasses.front()->GetPrefix() + ":" + m_vecSatisfiedClasses.front()->GetName();
				refIdent = m_vecSatisfiedClasses.front()->GetName() + "_" + std::string(m_pData->GetTitle());
			}




			// [rad] generate path and ident for atom
			void GetPathAndIdentAtom(const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				
				refPath = m_vecSatisfiedClasses.front()->GetPrefix() + ":" + m_vecSatisfiedClasses.front()->GetName();

				std::stringstream ssConv;
				OpenBabel::OBAtom* pAtom = m_pData;


				ssConv << m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId << "_" << pAtom->GetIdx();
				refIdent = "";
				ssConv >> refIdent;
			}





			// [rad] generate path and ident for bond
			void GetPathAndIdentBond(const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				
				refPath = m_vecSatisfiedClasses.front()->GetPrefix() + ":" + m_vecSatisfiedClasses.front()->GetName();

				std::stringstream ssConv;
				OpenBabel::OBBond* pBond = m_pData;

				OpenBabel::OBAtom* pAtomStart = pBond->GetBeginAtom();
				OpenBabel::OBAtom* pAtomEnd = pBond->GetEndAtom();


				ssConv << m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId << "_" <<
					pAtomStart->GetIdx() << "_"	<< pAtomEnd->GetIdx();

				refIdent = "";
				ssConv >> refIdent;
			}




			// [rad] generate path and ident for ring
			void GetPathAndIdentRing(const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				
				refPath = m_vecSatisfiedClasses.front()->GetPrefix() + ":" + m_vecSatisfiedClasses.front()->GetName();

				std::stringstream ssConv;
				OpenBabel::OBRing* pRing = m_pData;

				ssConv << m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId;

				std::vector<int>::iterator iter_path = pRing->_path.begin();
				while(iter_path != pRing->_path.end())
				{
					ssConv << "_" << (*iter_path);					

					iter_path++;
				}


				refIdent = "";
				ssConv >> refIdent;
			}





			// [rad] generate path and ident for object property pointing to atom
			void GetObjectPropertyPathAndIdentAtom(int iPosition, const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				refPath = m_vecSatisfiedObjectPropertiesAtom[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesAtom[iPosition].first->GetName();
				
				std::stringstream ssConv;

				ssConv << m_vecSatisfiedObjectPropertiesAtom[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId
					<< "_" << m_vecSatisfiedObjectPropertiesAtom[iPosition].second->m_pData->GetIdx();

				refIdent = "";
				ssConv >> refIdent;
			}





			// [rad] generate path and ident for bond
			void GetObjectPropertyPathAndIdentBond(int iPosition, const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				refPath = m_vecSatisfiedObjectPropertiesBond[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesBond[iPosition].first->GetName();
				
				std::stringstream ssConv;
				
				OpenBabel::OBAtom* pAtomStart = m_vecSatisfiedObjectPropertiesBond[iPosition].second->m_pData->GetBeginAtom();
				OpenBabel::OBAtom* pAtomEnd = m_vecSatisfiedObjectPropertiesBond[iPosition].second->m_pData->GetEndAtom();

				ssConv << m_vecSatisfiedObjectPropertiesBond[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId << "_" <<
					pAtomStart->GetIdx() << "_"	<< pAtomEnd->GetIdx();

				refIdent = "";
				ssConv >> refIdent;
			}





			// [rad] generate path and ident for ring
			void GetObjectPropertyPathAndIdentRing(int iPosition, const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				refPath = m_vecSatisfiedObjectPropertiesRing[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesRing[iPosition].first->GetName();
				
				std::stringstream ssConv;

				OpenBabel::OBRing* pRing = m_vecSatisfiedObjectPropertiesRing[iPosition].second->m_pData;

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





			// [rad] generate path and ident for object property pointing to molecule
			void GetObjectPropertyPathAndIdentMolecule(int iPosition, const std::string& refMoleculeId, std::string& refPath, std::string& refIdent)
			{
				refPath = m_vecSatisfiedObjectPropertiesMolecule[iPosition].first->GetPrefix() + ":" + 
					m_vecSatisfiedObjectPropertiesMolecule[iPosition].first->GetName();
				
				std::stringstream ssConv;

				ssConv << m_vecSatisfiedObjectPropertiesMolecule[iPosition].second->m_vecSatisfiedClasses.front()->GetName() << "_" << refMoleculeId;

				refIdent = "";
				ssConv >> refIdent;
			}

                

		public:

			T*																		m_pData;
			
			std::vector<Class*>														m_vecSatisfiedClasses;
			std::vector<std::pair<DataProperty*, std::string> >						m_vecSatisfiedDataProperties;
			
			std::vector<std::pair<ObjectProperty*, Relation<OpenBabel::OBMol>*> >	m_vecSatisfiedObjectPropertiesMolecule;
			std::vector<std::pair<ObjectProperty*, Relation<OpenBabel::OBAtom>*> >	m_vecSatisfiedObjectPropertiesAtom;
			std::vector<std::pair<ObjectProperty*, Relation<OpenBabel::OBBond>*> >	m_vecSatisfiedObjectPropertiesBond;
			std::vector<std::pair<ObjectProperty*, Relation<OpenBabel::OBRing>* > >	m_vecSatisfiedObjectPropertiesRing;
            
	};
}

#endif //!defined(OWL_RELATION_H)