#if !defined(OWL_CLASSRESOURCE_H)
#define OWL_CLASSRESOURCE_H

#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "Class.h"
#include "GenerateOptions.h"

#include <openbabel/mol.h>

namespace Owl
{
    class RelationMolecule;
    class RelationRing;
    class RelationBond;
    class RelationAtom;
	class RelationDescriptor;



	class ClassResource
	{
		public:
        
			ClassResource();
			ClassResource(const std::string& refName);
			ClassResource(const std::string& refName, const std::string& refAuxiliary);

			~ClassResource();

		public:

			const std::string&						GetName() const;
			const std::string&						GetAuxiliary() const;

			void									AddClass(const std::string& refPrefix, const std::string& refName);
			void									AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation = "equal");
			void									UnionStart();
			void									UnionEnd();
			void									IntersectionStart();
			void									IntersectionEnd();

			void									Print(std::ostream* pOutputStream);
			void									RetrieveClass(std::set<std::string>& refClasses);

			bool									HasMultipleDefault() const;

			
			bool                                    Satisfy(OpenBabel::OBMol* pMolecule,
                                                            RelationDescriptor* pRel,
                                                            GenerateOptions& refOptions);															
															
            bool                                    Satisfy(OpenBabel::OBMol* pMolecule,
                                                            RelationMolecule* pRel,
                                                            GenerateOptions& refOptions);

            bool                                    Satisfy(OpenBabel::OBAtom* pAtom,
                                                            RelationAtom* pRel,
                                                            GenerateOptions& refOptions);

            bool                                    Satisfy(OpenBabel::OBRing* pRing,
                                                            RelationRing* pRel,
                                                            GenerateOptions& refOptions);

            bool                                    Satisfy(OpenBabel::OBBond* pBond,
                                                            RelationBond* pRel,
                                                            GenerateOptions& refOptions);


		private:
	
			Class*									m_pCachedClass;
			
			std::string								m_sName;
			std::string								m_sAuxiliary;
			
			std::vector<Class*>						m_vecClasses;
	};
}

#endif //!defined(OWL_CLASSRESOURCE_H)