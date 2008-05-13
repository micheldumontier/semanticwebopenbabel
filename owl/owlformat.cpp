#include <openbabel/babelconfig.h>
#include <openbabel/obmolecformat.h>

#include <sstream>
#include <string>
#include <ctime>

#include "Configuration.h"


namespace OpenBabel
{
	//--
	// [rad] Our format
	class CFGOWLFormat : public OBMoleculeFormat
	{

		public:
        
			CFGOWLFormat();

		public:

			// [rad] Returns the string description of this format (provided)
			virtual const char*										Description();

			// [rad] Returns the specification URL of this format (provided)
			virtual const char*										SpecificationURL();

			// [rad] Returns the flags (properties) of this format (provided)
			virtual unsigned int									Flags();

			// [rad] Called whenever the framework wants to serialize the molecule using this format
			virtual bool											WriteMolecule(OBBase* pOb, OBConversion* pConv);

		protected:

			// [rad] Is called at the beginning of Serialization - performs object initialization (allocation, initialization, etc)
			bool													Initialize();

			// [rad] Is called at the end of Serialization - performs object finalization (de-alloc, etc)
			bool													Finalize();

			// [rad] Process command line params
			void													ProcessCommandlineParams(OBConversion* pConv);
		
		protected:

			// [rad] Molecule object/model we are working with
			OBMol*													m_pMolecule;
			
			Owl::Configuration*										m_ptrConfiguration;

			// [rad] Configuration file path
			std::string												m_sConfigurationFilePath;


			// [rad] Stream we serialize data to
			std::ostream*											m_pOutputStream;


	};




	//--
	// [rad] Global instance of our plugin/format.
	CFGOWLFormat g_CFGOWLFormat;




	//--
	CFGOWLFormat::CFGOWLFormat() :
		m_pMolecule(NULL), m_ptrConfiguration(NULL)
	{
		// [rad] Regiter .owl extension with this plugin
		OBConversion::RegisterFormat("owl", this);

		// [rad] Register 'q' command parameter with this plugin
		//OBConversion::RegisterOptionParam("q", this);
		OBConversion::RegisterOptionParam("q", this, 1, OBConversion::GENOPTIONS);

		// [rad] where to look for configuration file
		m_sConfigurationFilePath = "OwlPluginConfiguration.xml";
	}





	//--
	// [rad] Returns the string description of this format (provided)
	const char* CFGOWLFormat::Description()
	{
		return("OWL format  [Write-only]\n");
	}





	//--
	// [rad] Returns the specification URL of this format (provided)
	const char* CFGOWLFormat::SpecificationURL()
	{
		return("Specification URL\n");
	}





	//--
	// [rad] Returns the flags (properties) of this format (provided)
	// This format (plugin) is write-only.
	unsigned int CFGOWLFormat::Flags()
	{ 
		// [rad] writing only
		return(WRITEONEONLY);
	}





	//--
	// [rad] Process cmd line params
	void CFGOWLFormat::ProcessCommandlineParams(OBConversion* pConv)
	{
		if(pConv->IsOption("q", OBConversion::GENOPTIONS))
		{
			//pConv->GetOptionParams("q", OBConversion::GENOPTIONS);

			const std::map<std::string, std::string>* pMap = pConv->GetOptions(OBConversion::GENOPTIONS);
			
			// [rad] must have 1 param!
			if(pMap && pMap->size())
			{
				std::map<std::string, std::string>::const_iterator iter_param = pMap->find("q");
				if(iter_param != pMap->end())
				{
					// [rad] set new conf path
					m_sConfigurationFilePath = iter_param->second;
				}
			}
		}
		
	}

	//--
	// [rad] This method will be called whenever the framework wants to serialize the model/molecule
	// using this format. Essentially, this is plugin's entry point.
	bool CFGOWLFormat::WriteMolecule(OBBase* pBase, OBConversion* pConv)
	{
		// [rad] process command line params
		ProcessCommandlineParams(pConv);

		// [rad] Call initialization
		Initialize();

		// [rad] Cast molecule we want to serialize
		OBMol* pMolecule = dynamic_cast<OBMol*>(pBase);

		// [rad] store molecule 
		m_pMolecule = pMolecule;

		// [rad] Grab the output stream (we will use it to serialize the data)
		m_pOutputStream = pConv->GetOutStream();

		// [rad] Make sure stream is valid
		if(!m_pOutputStream) return(false);

		// [rad] set molecule and stream
		m_ptrConfiguration->SetMolecule(pMolecule);
		m_ptrConfiguration->SetOutputStream(m_pOutputStream);

		// [rad] add hydrogens if needed
		m_ptrConfiguration->AddHydrogens();	

		// [rad] write entities
		m_ptrConfiguration->WriteEntities();

		// [rad] write header
		m_ptrConfiguration->WriteHeader();

		// [rad] write ontology
		m_ptrConfiguration->WriteOntology();

		// [rad] Fwd decl classes
		m_ptrConfiguration->WriteBlockClasses();

		// [rad] Fwd decl data properties
		m_ptrConfiguration->WriteBlockDataProperties();

		// [rad] Fwd decl object properties
		m_ptrConfiguration->WriteBlockObjectProperties();

		// [rad] Write individuals
		m_ptrConfiguration->WriteBlockIndividuals();
		

		// [rad] write footer
		m_ptrConfiguration->WriteFooter();	

		// [rad] Call finalization
		Finalize();

		// [rad] true means we have successfuly serialized our object
		return(true);
	}




	//--
	// [rad] Initialization method
	bool CFGOWLFormat::Initialize()
	{
		// [rad] create configuration
		m_ptrConfiguration = new Owl::Configuration();

		// [rad] Load configuration
		if(!m_ptrConfiguration->Load(m_sConfigurationFilePath)) return(false);

		return(true);
	}




	//--
	// [rad] Finalization method
	bool CFGOWLFormat::Finalize()
	{
	
		// [rad] delete configuration
		if(m_ptrConfiguration)
		{
			delete(m_ptrConfiguration);
			m_ptrConfiguration = NULL;
		}
		
		return(true);
	}

}
