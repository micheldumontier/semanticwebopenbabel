#if !defined(OWL_DATAPROPERTYRESOURCE_H)
#define OWL_DATAPROPERTYRESOURCE_H

#include <string>
#include <vector>
#include <set>
#include <sstream>

#include "DataProperty.h"
#include "GenerateOptions.h"

#include <openbabel/mol.h>


namespace Owl
{
    class RelationMolecule;
    class RelationRing;
    class RelationBond;
    class RelationAtom;
	class RelationDescriptor;

	class DataPropertyResource
	{
		public:
        
			DataPropertyResource();
			DataPropertyResource(const std::string& refFrom, const std::string& refTo);
			~DataPropertyResource();

		public:

			const std::string&						GetFrom() const;
			const std::string&						GetTo() const;

			void									AddDataProperty(const std::string& refPrefix, const std::string& refName, const std::string& refType);
			void									AddOption(const std::string& refPrefix, const std::string& refName);

			void									Print(std::ostream* pOutputStream);
			void									RetrieveProperty(std::set<std::string>& refDataProperties);


            
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

			bool									HasSingleDefault() const;
			bool									HasMultipleDefault() const;
		

		private:
	
			DataProperty*							m_pCachedDataProperty;

			std::string								m_sFrom;
			std::string								m_sTo;

			std::vector<DataProperty*>				m_vecDataProperties;
	};
}

#endif //!defined(OWL_DATAPROPERTYRESOURCE_H)