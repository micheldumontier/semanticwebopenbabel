#include "DataPropertyOption.h"

namespace Owl
{
	//--
	DataPropertyOption::DataPropertyOption() : m_sPrefix(""), m_sName("")
	{
	}



	//--
	DataPropertyOption::DataPropertyOption(const std::string& refPrefix, const std::string& refName) : m_sPrefix(refPrefix), m_sName(refName)
	{
	}



	//--
	const std::string& DataPropertyOption::GetPrefix() const
	{
		return(m_sPrefix);
	}



	//--
	const std::string& DataPropertyOption::GetName() const
	{
		return(m_sName);
	}



	//--
	void DataPropertyOption::Print(std::ostream* pOutputStream, int iIdent)
	{
		if(1 == iIdent)
		{
			*pOutputStream << "    <rdfs:domain rdf:resource=\"&" << m_sPrefix << ";" << m_sName << "\" />" << std::endl;
		}
		else
		{
			for(int i = 0; i < iIdent; i++)
			{
				*pOutputStream << "    ";
			}

			*pOutputStream << "<rdf:Description rdf:about=\"&" << m_sPrefix << ";" << m_sName << "\" />" << std::endl;
		}
	}

}