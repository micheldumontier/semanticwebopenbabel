#if !defined(OWL_RELATIONATOM_H)
#define OWL_RELATIONATOM_H

#include <openbabel/mol.h>

namespace Owl
{
    class ObjectProperty;
    class Class;
    class DataProperty;
    
    class RelationMolecule;
    class RelationRing;
    class RelationBond;
    
    class RelationAtom
    {        
        public:
        
            void                AddRelationMolecule(ObjectProperty* pProperty, RelationMolecule* pRel);
            void                AddRelationAtom(ObjectProperty* pProperty, RelationAtom* pRel);
            void                AddRelationRing(ObjectProperty* pProperty, RelationRing* pRel);
            void                AddRelationBond(ObjectProperty* pProperty, RelationBond* pRel);
            
            void                Clear();

            void                AddClass(Class* pClass);
            
            void                AddData(DataProperty* pProperty, const std::string& refString);
            
            
            
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
                                                            
            
            void                GetDataProperty(int iPosition, 
                                                    std::string& refPath, 
                                                    std::string& refType, 
                                                    std::string& refValue);
                                                    
                                                    
                                                    
			void                GetPathAndIdentAtom(const std::string& refMoleculeId, 
                                                    std::string& refPath, 
                                                    std::string& refIdent);
                                                    
                                                    
			void                GetClassPath(int iPosition, std::string& refIdent);
			
                                                    
            OpenBabel::OBAtom*  GetAtom() const;
            
        public:
        
            OpenBabel::OBAtom*                                              m_pAtom;
        
            
            std::vector<Class*>												m_vecSatisfiedClasses;
			std::vector<std::pair<DataProperty*, std::string> >				m_vecSatisfiedDataProperties;

            std::vector<std::pair<ObjectProperty*, RelationMolecule*> >     m_vecSatisfiedObjectPropertiesMolecule;
            std::vector<std::pair<ObjectProperty*, RelationBond*> >         m_vecSatisfiedObjectPropertiesBond;
            std::vector<std::pair<ObjectProperty*, RelationAtom*> >         m_vecSatisfiedObjectPropertiesAtom;
            std::vector<std::pair<ObjectProperty*, RelationRing*> >         m_vecSatisfiedObjectPropertiesRing;
            
            
    };
}

#endif //!defined(OWL_RELATIONATOM_H)