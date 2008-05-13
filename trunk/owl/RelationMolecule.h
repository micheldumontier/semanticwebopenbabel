#if !defined(OWL_RELATIONMOLECULE_H)
#define OWL_RELATIONMOLECULE_H

#include <openbabel/mol.h>

namespace Owl
{
    class ObjectProperty;
    class Class;
    class DataProperty;
    class RelationAtom;
    class RelationRing;
    class RelationBond;
	class RelationDescriptor;
    
    class RelationMolecule
    {        
        public:
        
            void                AddRelationMolecule(ObjectProperty* pProperty, RelationMolecule* pRel);
            void                AddRelationAtom(ObjectProperty* pProperty, RelationAtom* pRel);
            void                AddRelationRing(ObjectProperty* pProperty, RelationRing* pRel);
            void                AddRelationBond(ObjectProperty* pProperty, RelationBond* pRel);
			void				AddRelationDescriptor(ObjectProperty* pProperty, RelationDescriptor* pRel);
            
            void                AddClass(Class* pClass);
            
            void                AddData(DataProperty* pProperty, const std::string& refString);
            
            void                Clear();
            
            
			void                GetPathAndIdentMolecule(std::string& refPath, std::string& refIdent);
            void                GetClassPath(int iPosition, std::string& refIdent);
            
            
            bool                HasExtraClasses();
			bool                HasDataProperties();
            bool                HasObjectProperties();
            bool                IsTrivial();

    
    
    
            void                GetObjectPropertyPathAndIdentMolecule(int iPosition, 
                                                                const std::string& refMoleculeId, 
                                                                std::string& refPath, 
                                                                std::string& refIdent);
                                                                
                                                                
            void                GetObjectPropertyPathAndIdentRing(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent);
                                                            
                 
            void                GetObjectPropertyPathAndIdentBond(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent);
                                                            
                                                            
            void                GetObjectPropertyPathAndIdentAtom(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent);
															
															
			void				GetObjectPropertyPathAndIdentDescriptor(int iPosition, 
                                                            const std::string& refMoleculeId, 
                                                            std::string& refPath, 
                                                            std::string& refIdent);




            void                GetDataProperty(int iPosition, 
                                                    std::string& refPath, 
                                                    std::string& refType, 
                                                    std::string& refValue);    
    
            OpenBabel::OBMol*   GetMolecule() const;
    
        public:
        
            OpenBabel::OBMol*                                               m_pMol;
        
            std::vector<Class*>												m_vecSatisfiedClasses;
			std::vector<std::pair<DataProperty*, std::string> >				m_vecSatisfiedDataProperties;

            std::vector<std::pair<ObjectProperty*, RelationMolecule*> >     m_vecSatisfiedObjectPropertiesMolecule;
            std::vector<std::pair<ObjectProperty*, RelationBond*> >         m_vecSatisfiedObjectPropertiesBond;
            std::vector<std::pair<ObjectProperty*, RelationAtom*> >         m_vecSatisfiedObjectPropertiesAtom;
            std::vector<std::pair<ObjectProperty*, RelationRing*> >         m_vecSatisfiedObjectPropertiesRing;
			std::vector<std::pair<ObjectProperty*, RelationDescriptor*> >	m_vecSatisfiedObjectPropertiesDescriptor;
            
            
    };
}

#endif //!defined(OWL_RELATIONMOLECULE_H)