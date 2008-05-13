#if !defined(OWL_CLASSUNION_H)
#define OWL_CLASSUNION_H

#include "IClassEntry.h"

#include <vector>

namespace Owl
{
	class ClassUnion : public IClassEntry
	{

		public:
        
			ClassUnion();
			virtual ~ClassUnion();

		private:
        
			const std::string&									VGetDescription() const;

			bool												VIsLeaf();

			void												VUnionStart();
			bool												VUnionEnd();

			void												VIntersectionStart();
			bool												VIntersectionEnd();

			void												VAddCondition(const std::string& refOn, const std::string& refValue, 
																					const std::string& refOperation);

			bool												VSatisfy(OpenBabel::OBMol* pMolecule);
			bool												VSatisfy(OpenBabel::OBAtom* pAtom);
			bool												VSatisfy(OpenBabel::OBRing* pRing);
			bool												VSatisfy(OpenBabel::OBBond* pBond);
			bool												VSatisfyDescriptor(OpenBabel::OBMol* pMolecule, 
																					const std::string& refAux);

		private:
        
			IClassEntry*										m_pCurrentActive;
			
			std::vector<IClassEntry*>							m_vecClassEntries;
	};
}

#endif //!defined(OWL_CLASSUNION_H)