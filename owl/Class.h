#if !defined(OWL_CLASS_H)
#define OWL_CLASS_H

#include <string>
#include <set>
#include <sstream>
#include <openbabel/mol.h>

#include "ClassIntersection.h"

namespace Owl
{
	class Class
	{


		public:
        
			Class();
			Class(const std::string& refPrefix, const std::string& refName);
			
			~Class();
			
		public:
        
			const std::string&							GetPrefix() const;
			const std::string&							GetName() const;

			void										UnionStart();
			void										UnionEnd();

			void										IntersectionStart();
			void										IntersectionEnd();

			bool										IsDefault() const;

			void										AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation);

			void										Print(std::ostream* pOutputStream);
			void										RetrieveClass(std::set<std::string>& refClasses);

			bool										SatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux);
			bool										Satisfy(OpenBabel::OBMol* pMolecule);
			bool										Satisfy(OpenBabel::OBAtom* pAtom);
			bool										Satisfy(OpenBabel::OBBond* pBond);
			bool										Satisfy(OpenBabel::OBRing* pRing);

		private:
		
			std::string									m_sPrefix;
			std::string									m_sName;

			// [rad] root - intersection (default), if no union/intersection is mentioned, intersection is assumed
			ClassIntersection*							m_ptrRoot;
	};
}

#endif //!defined(OWL_CLASS_H)