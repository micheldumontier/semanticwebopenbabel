#include "GenerateOptions.h"

namespace Owl
{
	//--
	GenerateOptions::GenerateOptions() :
		m_bGenerateHydrogens(true),
		m_bGenerateAssertions(true),
		m_bGenerateAllDifferent(true),
		m_bGenerateForceSingleType(true),
		m_bGenerateTimeStamp(true),
		m_bGenerateComments(true),
		m_bGenerateUseMultipleDefault(false),
		m_bGenerateInchiMapping(true)
	{

	}

	//--
	void GenerateOptions::Reset()
	{
		m_bGenerateHydrogens = true;
		m_bGenerateAssertions = true;
		m_bGenerateAllDifferent = true;
		m_bGenerateForceSingleType = true;
		m_bGenerateTimeStamp = true;
		m_bGenerateComments = true;
		m_bGenerateUseMultipleDefault = false;
		m_bGenerateInchiMapping = true;
	}
}