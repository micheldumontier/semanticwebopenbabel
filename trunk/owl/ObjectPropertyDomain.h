#if !defined(OWL_OBJECTPROPERTYDOMAIN_H)
#define OWL_OBJECTPROPERTYDOMAIN_H

#include <string>

#include "ObjectPropertyIntersection.h"

namespace Owl
{
    class RelationMolecule;
    class RelationAtom;
    class RelationRing;
    class RelationBond;
	class RelationDescriptor;

    class ObjectPropertyDomain
	{
	
        public:
        
			ObjectPropertyDomain();
			ObjectPropertyDomain(const std::string& refPrefix, const std::string& refName);

            ~ObjectPropertyDomain();
			
		public:
        
			const std::string&							GetPrefix() const;
			const std::string&							GetName() const;

			void										UnionStart();
			void										UnionEnd();

			void										IntersectionStart();
			void										IntersectionEnd();

			void										AddCondition(const std::string& refOn, 
                                                                        const std::string& refValue, 
                                                                        const std::string& refOperation);

            
			bool										Satisfy(RelationDescriptor* pRelDescriptor);
            bool                                        Satisfy(RelationMolecule* pRelMolecule);
            bool                                        Satisfy(RelationAtom* pRelAtom);
            bool                                        Satisfy(RelationBond* pRelBond);
            bool                                        Satisfy(RelationRing* pRelRing);

		private:

			std::string									m_sPrefix;
			std::string									m_sName;

			// [rad] root - intersection (default), if no union/intersection is mentioned, intersection is assumed
            ObjectPropertyIntersection*                 m_ptrRoot;
	};
}

#endif //!defined(OWL_OBJECTPROPERTYDOMAIN_H)