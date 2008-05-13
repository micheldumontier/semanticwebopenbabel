#include "Import.h"

namespace Owl
{
	//--
	Import::Import() :
	m_sPath("")
	{

	}



	//--
	Import::Import(const std::string& refPath) :
	m_sPath(refPath)
	{

	}


	
	//--
	const std::string& Import::GetPath() const
	{
		return(m_sPath);
	}


	//--
	void Import::Print(std::ostream* pOutputStream)
	{
		*pOutputStream << "        <owl:Ontology rdf:about=\"" << m_sPath << "\" />" << std::endl;
	}
}