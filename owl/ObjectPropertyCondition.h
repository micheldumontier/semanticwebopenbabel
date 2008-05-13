#if !defined(OWL_OBJECTPROPERTYCONDITION_H)
#define OWL_OBJECTPROPERTYCONDITION_H

#include "IObjectPropertyEntry.h"

namespace Owl
{
    class RelationMolecule;
    class RelationAtom;
    class RelationBond;
    class RelationRing;
	class RelationDescriptor;

	class ObjectPropertyCondition : public IObjectPropertyEntry
	{
		public:

			ObjectPropertyCondition();
			ObjectPropertyCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation);
			
		public:

			const std::string&				GetOn() const;
			const std::string&				GetValue() const;
			const std::string&				GetOperation() const;

		private:

			const std::string&				VGetDescription() const;
			bool							VIsLeaf();

			
			bool							VSatisfy(RelationDescriptor* pRelationDescriptor);
			bool							VSatisfy(RelationMolecule* pRelationMolecule);
			bool							VSatisfy(RelationAtom* pRelationAtom);
			bool							VSatisfy(RelationBond* pRelationBond);
			bool							VSatisfy(RelationRing* pRelationRing);

		private:
			
			bool							ProcessInt(int iValue);
			bool							ProcessBool(bool bValue);
			bool							ProcessString(const std::string& sValue);
			bool							ProcessDouble(double dValue);

			bool							ProcessDescriptor(RelationMolecule* pRelationMolecule);

		private:

			std::string						m_sOn;
			std::string						m_sValue;
			std::string						m_sOperation;
	};
}

#endif //!defined(OWL_OBJECTPROPERTYCONDITION_H)