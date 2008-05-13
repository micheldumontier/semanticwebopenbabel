#if !defined(OWL_CONFIGURATION_H)
#define OWL_CONFIGURATION_H

#include <vector>
#include <string>
#include <map>
#include <sstream>
#include <ctime>

#include <openbabel/mol.h>

#include "Import.h"
#include "URI.h"
#include "ClassResource.h"
#include "DataPropertyResource.h"
#include "ObjectPropertyResource.h"
#include "GenerateOptions.h"

#include "tinyxml/tinyxml.h"

namespace Owl
{
    class RelationMolecule;
    class RelationAtom;
    class RelationBond;
    class RelationRing;
	class RelationDescriptor;

	class Configuration
	{

		// [rad] Enum used for xml config file parsing 
        enum XmlBlock
		{
			XmlBlockNone							= 0,
			XmlBlockPlugin							= 1,
			XmlBlockGenerate						= 2,
			XmlBlockURIs							= 3,
			XmlBlockImports							= 4,
			XmlBlockBase							= 5,
			XmlBlockClasses							= 6,
			XmlBlockClassResource					= 7,
			XmlBlockClass							= 8,
			XmlBlockClassUnion						= 9,
			XmlBlockClassIntersection				= 10,
			XmlBlockDataProperties					= 11,
			XmlBlockDataPropertyResource			= 12,
			XmlBlockDataProperty					= 14,
			XmlBlockObjectProperties				= 15,
			XmlBlockObjectPropertyResource			= 16,
			XmlBlockObjectProperty					= 17,
			XmlBlockObjectPropertyDomain			= 18,
			XmlBlockObjectPropertyRange				= 19,
			XmlBlockObjectPropertyUnion				= 20,
			XmlBlockObjectPropertyIntersection		= 21,
		};
		

		public:
	
			Configuration();
            ~Configuration();

		public:

			// [rad] reset configuration
			void									Reset();

			// [rad] load configuration from a file
			bool									Load(const std::string& refConfFile);

			// [rad] adds missing hydrogens
			bool									AddHydrogens();

			
			// [rad] Set output stream
			void									SetOutputStream(std::ostream* pOutputStream);
			

			// [rad] Set molecule
			void									SetMolecule(OpenBabel::OBMol* pMolecule);
			

			// [rad] Write entities
			void									WriteEntities();

			// [rad] Write header
			void									WriteHeader();

			// [rad] Write footer
			void									WriteFooter();

			// [rad] Write ontology
			void									WriteOntology();

			// [rad] Write classes
			void									WriteBlockClasses();

			// [rad] Write data properties
			void									WriteBlockDataProperties();

			// [rad] Write object properties
			void									WriteBlockObjectProperties();

			// [rad] Write individuals
			void									WriteBlockIndividuals();

			
		private:

			// [rad] Returns whether a given attribute value is set to true / yes / 1
			bool									IsAttributeEnabled(const char* pAttributeVal);

			// [rad] Convert attribute to bool	
			bool									AttributeToBool(const std::string& refAttribute);

			// [rad] Convert attribute to an int
			int										AttributeToInt(const std::string& refAttribute);

		private:

			void									ProcessClassUnion(TiXmlElement* pNodeUnion);
			void									ProcessClassIntersection(TiXmlElement* pNodeIntersection);

			void									ProcessObjectPropertyUnion(TiXmlElement* pNodeUnion);
			void									ProcessObjectPropertyIntersection(TiXmlElement* pNodeIntersection);

			inline void								WriteSpace();
			
			inline void								WriteComment(const std::string& refComment, const std::string& refNamespace, const std::string& refItem);

			inline void								WriteCommentBlock(const std::string& refComment);


			inline void								WriteIndividualDescriptor(RelationDescriptor* pRelDescriptor);
            inline void								WriteIndividualMolecule(RelationMolecule* pRelMolecule);
			inline void								WriteIndividualAtom(RelationAtom* pRelAtom);
			inline void								WriteIndividualRing(RelationRing* pRelRing);
			inline void								WriteIndividualBond(RelationBond* pRelBond);
            
		private:

			// [rad] generate options
			GenerateOptions							m_objOptions;

			// [rad] Base
			std::string								m_sBase;
			std::string								m_sBaseOntology;


            std::vector<URI*>                       m_vecURIs;
			std::vector<Import*>					m_vecImports;
			std::vector<ClassResource*>             m_vecClassResources;
			std::vector<DataPropertyResource*>      m_vecDataPropertyResources;
			std::vector<ObjectPropertyResource*>	m_vecObjectPropertyResources;

			// [rad] Block map
			std::map<std::string, XmlBlock>			m_mapXmlBlocks;

			// [rad] Counts ids
			std::map<std::string, int>				m_mapIds;

			// [rad] output stream
			std::ostream*							m_pOutputStream;

			// [rad] Molecule
			OpenBabel::OBMol*						m_pMolecule;
	};
}

#endif //!defined(OWL_CONFIGURATION_H)