#if !defined(OWL_RELATIONBOND_H)
#define OWL_RELATIONBOND_H

#include <openbabel/mol.h>

namespace Owl
{
    class ObjectProperty;
    class Class;
    class DataProperty;
    class RelationAtom;
    class RelationRing;
    class RelationMolecule;

    class RelationBond
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
       
                                                    
			void                GetClassPath(int iPosition, std::string& refIdent);        
            
                
                    
            void                GetPathAndIdentBond(const std::string& refMoleculeId, 
                                                        std::string& refPath, 
                                                        std::string& refIdent);
                                                        
            OpenBabel::OBBond*  GetBond() const;
                      
        public:
        
            OpenBabel::OBBond*                                              m_pBond;
        
            
            std::vector<Class*>												m_vecSatisfiedClasses;
			std::vector<std::pair<DataProperty*, std::string> >				m_vecSatisfiedDataProperties;

            std::vector<std::pair<ObjectProperty*, RelationMolecule*> >     m_vecSatisfiedObjectPropertiesMolecule;
            std::vector<std::pair<ObjectProperty*, RelationBond*> >         m_vecSatisfiedObjectPropertiesBond;
            std::vector<std::pair<ObjectProperty*, RelationAtom*> >         m_vecSatisfiedObjectPropertiesAtom;
            std::vector<std::pair<ObjectProperty*, RelationRing*> >         m_vecSatisfiedObjectPropertiesRing;
            
            
    };
}

#endif //!defined(OWL_RELATIONBOND_H)