#if !defined(OWL_CLASSENTRY_H)
#define OWL_CLASSENTRY_H

#include <string>
#include <openbabel/mol.h>

namespace Owl
{
	class IClassEntry
	{		
		public:
		
			virtual							~IClassEntry() { }
			
			
		private:
		
			virtual const std::string&		VGetDescription() const = 0;

			virtual bool					VIsLeaf() = 0;

			virtual void					VUnionStart(){}
			virtual bool					VUnionEnd(){ return(false); }

			virtual void					VIntersectionStart(){}
			virtual bool					VIntersectionEnd(){ return(false); }

			virtual void					VAddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation){}

			virtual bool					VSatisfy(OpenBabel::OBMol* pMolecule) = 0;
			virtual bool					VSatisfy(OpenBabel::OBAtom* pAtom) = 0;
			virtual bool					VSatisfy(OpenBabel::OBBond* pBond) = 0;
			virtual bool					VSatisfy(OpenBabel::OBRing* pRing) = 0;
			virtual bool					VSatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux) = 0;

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


			bool							SatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refAux)
			{
				return(VSatisfyDescriptor(pMolecule, refAux));
			}
			
			bool							Satisfy(OpenBabel::OBMol* pMolecule)
			{
				return(VSatisfy(pMolecule));
			}

			bool							Satisfy(OpenBabel::OBAtom* pAtom)
			{
				return(VSatisfy(pAtom));
			}

			bool							Satisfy(OpenBabel::OBBond* pBond)
			{
				return(VSatisfy(pBond));
			}

			bool							Satisfy(OpenBabel::OBRing* pRing)
			{
				return(VSatisfy(pRing));
			}
	};
}

#endif //!defined(OWL_CLASSENTRY_H)