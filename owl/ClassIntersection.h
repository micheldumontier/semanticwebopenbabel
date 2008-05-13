#if !defined(OWL_CLASSINTERSECTION_H)
#define OWL_CLASSINTERSECTION_H

#include "IClassEntry.h"

#include <vector>

namespace Owl
{
	class ClassIntersection : public IClassEntry
	{
		public:

			ClassIntersection();
			virtual ~ClassIntersection();

		public:

			bool												IsDefault() const;

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

#endif //!defined(OWL_CLASSINTERSECTION_H)