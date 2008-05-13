#include "URI.h"

namespace Owl
{
	//--
	URI::URI() :
	m_sPrefix(""),
	m_sPath(""),
	m_bSpecial(false)
	{

	}



	//--
	URI::URI(const std::string& refPrefix, const std::string& refPath) :
	m_sPrefix(refPrefix),
	m_sPath(refPath),
	m_bSpecial(false)
	{

	}


	//--
	URI::URI(const std::string& refPrefix, const std::string& refPath, bool bSpecial) :
	m_sPrefix(refPrefix),
	m_sPath(refPath),
	m_bSpecial(bSpecial)
	{

	}


	//--
	const std::string& URI::GetPath() const
	{
		return(m_sPath);
	}



	//--
	const std::string& URI::GetPrefix() const
	{
		return(m_sPrefix);
	}


	//--
	void URI::PrintEntity(std::ostream* pOutputStream)
	{
		if(m_bSpecial)
		{
			*pOutputStream << "         <!ENTITY " << m_sPrefix << " \"" << m_sPath << "\" >" << std::endl;
		}
		else
		{
			*pOutputStream << "         <!ENTITY " << m_sPrefix << " \"" << m_sPath << "#\" >" << std::endl;
		}
	}


	//--
	void URI::PrintURI(std::ostream* pOutputStream)
	{
		if(m_bSpecial)
		{
			*pOutputStream << "         xmlns:" << m_sPrefix << "=\"" << m_sPath << "\"" << std::endl;
		}
		else
		{
			*pOutputStream << "         xmlns:" << m_sPrefix << "=\"" << m_sPath << "#\"" << std::endl;
		}
	}
}