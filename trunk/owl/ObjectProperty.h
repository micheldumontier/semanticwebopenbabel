#if !defined(OWL_OBJECTPROPERTY_H)
#define OWL_OBJECTPROPERTY_H

#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "ObjectPropertyDomain.h"
#include "ObjectPropertyRange.h"

namespace Owl
{
	class ObjectPropertyDomain;
	class ObjectPropertyRange;
    class RelationMolecule;
    class RelationAtom;
    class RelationBond;
    class RelationRing;
	class RelationDescriptor;

	class ObjectProperty
	{
        public:

			ObjectProperty();
			ObjectProperty(const std::string& refPrefix, 
                            const std::string& refName, 
                            const std::string& refAuxiliary);
            
			~ObjectProperty();
			
		public:

			const std::string&							GetPrefix() const;
			const std::string&							GetName() const;
			const std::string&							GetAuxiliary() const;

			bool										IsDefault() const;

			void										AddDomain(const std::string& refPrefix, const std::string& refName);
			void										AddRange(const std::string& refPrefix, const std::string& refName);
		
			void										AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation);

			void										UnionStart();
			void										UnionEnd();
			void										IntersectionStart();
			void										IntersectionEnd();

			void										Print(std::ostream* pOutputStream);
			void										RetrieveProperty(std::set<std::string>& refObjectProperties);

            
            
            
            bool										Satisfy(RelationMolecule* pRelMolecule, 
																RelationDescriptor* pRelDescriptor, 
																bool bForward = true);
																
            bool										Satisfy(RelationMolecule* pRelMolecule, 
																RelationAtom* pRelAtom, 
																bool bForward = true);
			
			bool										Satisfy(RelationMolecule* pRelMolecule, 
																RelationBond* pRelBond, 
																bool bForward = true);
			
			bool										Satisfy(RelationMolecule* pRelMolecule, 
																RelationRing* pRelRing, 
																bool bForward = true);

			bool										Satisfy(RelationAtom* pRelAtom, 
																RelationBond* pRelBond, 
																bool bForward = true);
																
			bool										Satisfy(RelationAtom* pRelAtom, 
																RelationRing* pRelRing, 
																bool bForward = true);

			bool										Satisfy(RelationBond* pRelBond, 
																RelationRing* pRelRing, 
																bool bForward = true);

			bool										Satisfy(RelationMolecule* pRelMolecule1, 
																RelationMolecule* pRelMolecule2, 
																bool bForward = true);
																
			bool										Satisfy(RelationBond* pRelBond1, 
																RelationBond* pRelBond2, 
																bool bForward = true);
																
			bool										Satisfy(RelationRing* pRelRing1, 
																RelationRing* pRelRing2, 
																bool bForward = true);
			
			bool										Satisfy(RelationAtom* pRelAtom1, 
																RelationAtom* pRelAtom2, 
																bool bForward = true);
																
			bool										SatisfyConnected(RelationAtom* pRelAtom1, 
																		RelationAtom* pRelAtom2, 
																		bool bForward = true);
                                                                                                                             
                                                                        

		private:

			std::string									m_sPrefix;
			std::string									m_sName;
			std::string									m_sAuxiliary;

			
			ObjectPropertyDomain*						m_ptrDomain;
			ObjectPropertyRange*						m_ptrRange;

			bool										m_bDomainCurrent;
			bool										m_bRangeCurrent;
	};
}

#endif //!defined(OWL_OBJECTPROPERTY_H)