#if !defined(OWL_OBJECTPROPERTYRESOURCE_H)
#define OWL_OBJECTPROPERTYRESOURCE_H

#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "ObjectProperty.h"
#include "GenerateOptions.h"

namespace Owl
{
    class RelationAtom;
    class RelationMolecule;
    class RelationBond;
    class RelationRing;
	class RelationDescriptor;

    class ObjectPropertyResource
	{
	
		public:

			ObjectPropertyResource();
			ObjectPropertyResource(const std::string& refFrom, const std::string& refTo, const std::string& refConstraint = "none");
            
            ~ObjectPropertyResource();

		public:

			const std::string&						GetFrom() const;
			const std::string&						GetTo() const;
			const std::string&						GetConstraint() const;

			void									AddObjectProperty(const std::string& refPrefix, const std::string& refName, const std::string& refAuxiliary = "");

			void									AddDomain(const std::string& refPrefix, const std::string& refName);
			void									AddRange(const std::string& refPrefix, const std::string& refName);

			void									AddCondition(const std::string& refOn, const std::string& refValue, const std::string& refOperation = "equal");

			
			void									UnionStart();
			void									UnionEnd();
			void									IntersectionStart();
			void									IntersectionEnd();

			void									Print(std::ostream* pOutputStream);
			void									RetrieveProperty(std::set<std::string>& refObjectProperties);


			bool									Satisfy(RelationMolecule* pRelMolecule, 
                                                            RelationDescriptor* pRelDescriptor,
                                                            GenerateOptions& refOptions,
                                                            bool bForward = true);   
															
            bool									Satisfy(RelationMolecule* pRelMolecule, 
                                                            RelationAtom* pRelAtom,
                                                            GenerateOptions& refOptions,
                                                            bool bForward = true);                                               

			bool									Satisfy(RelationMolecule* pRelMolecule, 
                                                            RelationBond* pRelBond, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationMolecule* pRelMolecule, 
                                                            RelationRing* pRelRing, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationAtom* pRelAtom, 
                                                            RelationBond* pRelBond, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationAtom* pRelAtom, 
                                                            RelationRing* pRelRing, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationBond* pRelBond, 
                                                            RelationRing* pRelRing, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationMolecule* pRelMolecule, 
                                                            RelationMolecule* pRelMolecule2,
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationAtom* pRelAtom, 
                                                            RelationAtom* pRelAtom2, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationBond* pRelBond, 
                                                            RelationBond* pRelBond2, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);

			bool									Satisfy(RelationRing* pRelRing, 
                                                            RelationRing* pRelRing2, 
                                                            GenerateOptions& refOptions, 
                                                            bool bForward = true);


            
		private:

			bool									HasMultipleDefault() const;
			
		private:
	
			ObjectProperty*							m_pCachedObjectProperty;
			
			std::string								m_sFrom;
			std::string								m_sTo;
			std::string								m_sConstraint;

            std::vector<ObjectProperty*>			m_vecObjectProperties;
	};
}

#endif //!defined(OWL_OBJECTPROPERTYRESOURCE_H)