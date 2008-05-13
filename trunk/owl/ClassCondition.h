#if !defined(OWL_CLASSCONDITION_H)
#define OWL_CLASSCONDITION_H

#include "IClassEntry.h"

namespace Owl
{
	class ClassCondition : public IClassEntry
	{
		public:

			ClassCondition();
			ClassCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation);
			
		public:

			const std::string&				GetOn() const;
			const std::string&				GetValue() const;
			const std::string&				GetOperation() const;

		private:

			const std::string&				VGetDescription() const;
			bool							VIsLeaf();

			bool							VSatisfy(OpenBabel::OBMol* pMolecule);
			bool							VSatisfy(OpenBabel::OBAtom* pAtom);
			bool							VSatisfy(OpenBabel::OBBond* pBond);
			bool							VSatisfy(OpenBabel::OBRing* pRing);
			bool							VSatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux);

		private:
			
			void							Initialize();

			bool							ProcessInt(int iValue);
			bool							ProcessBool(bool bValue);
			bool							ProcessString(const std::string& sValue);
			bool							ProcessDouble(double dValue);

			bool							ProcessDescriptor(OpenBabel::OBMol* pMolecule);

		private:

			std::string						m_sOn;
			std::string						m_sValue;
			std::string						m_sOperation;
			
	};
}

#endif //!defined(OWL_CLASSCONDITION_H)