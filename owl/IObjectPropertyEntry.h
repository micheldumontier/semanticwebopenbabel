#if !defined(OWL_OBJECTPROPERTYENTRY_H)
#define OWL_OBJECTPROPERTYENTRY_H

#include <string>
#include <openbabel/mol.h>

namespace Owl
{
    
    class RelationMolecule;
    class RelationAtom;
    class RelationBond;
    class RelationRing;
	class RelationDescriptor;



	class IObjectPropertyEntry
	{
		public:
		
			virtual ~IObjectPropertyEntry() { }
			

		private:

			virtual const std::string&									VGetDescription() const = 0;

			virtual bool												VIsLeaf() = 0;

			virtual void												VUnionStart(){}
			virtual bool												VUnionEnd(){ return(false); }

			virtual void												VIntersectionStart(){}
			virtual bool												VIntersectionEnd(){ return(false); }

			virtual void												VAddCondition(const std::string& refOn, const std::string& refValue, 
																					const std::string& refOperation){}

			virtual bool												VSatisfy(RelationDescriptor* pRelationDescriptor) = 0;
            virtual bool												VSatisfy(RelationMolecule* pRelationMolecule) = 0;
			virtual bool												VSatisfy(RelationAtom* pRelationAtom) = 0;
			virtual bool												VSatisfy(RelationBond* pRelationBond) = 0;
			virtual bool												VSatisfy(RelationRing* pRelationRing) = 0;                                                




		public:

			const std::string&				GetDescription() const
			{
				return(VGetDescription());
			}

			void							UnionStart()
			{
				return(VUnionStart());
			}

			bool							UnionEnd()
			{
				return(VUnionEnd());
			}

			void							IntersectionStart()
			{
				return(VIntersectionStart());
			}

			bool							IntersectionEnd()
			{
				return(VIntersectionEnd());
			}




			void							AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation)
			{
				return(VAddCondition(refOn, refValue, refOperation));
			}




			bool							IsLeaf()
			{
				return(VIsLeaf());
			}



			bool							Satisfy(RelationDescriptor* pRelationDescriptor)
			{
				return(VSatisfy(pRelationDescriptor));
			}

			bool							Satisfy(RelationMolecule* pRelationMolecule)
			{
				return(VSatisfy(pRelationMolecule));
			}

			bool							Satisfy(RelationAtom* pRelationAtom)
			{
				return(VSatisfy(pRelationAtom));
			}

			bool							Satisfy(RelationBond* pRelationBond)
			{
				return(VSatisfy(pRelationBond));
			}

			bool							Satisfy(RelationRing* pRelationRing)
			{
				return(VSatisfy(pRelationRing));
			}
	};
}

#endif //!defined(OWL_OBJECTPROPERTYENTRY_H)