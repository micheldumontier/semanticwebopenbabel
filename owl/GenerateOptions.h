#if !defined(OWL_GENERATE_OPTIONS_H)
#define OWL_GENERATE_OPTIONS_H


namespace Owl
{
	class GenerateOptions
	{
		public:
			GenerateOptions();

		public:

			void									Reset();

		public:
	
			bool									m_bGenerateHydrogens;
			bool									m_bGenerateAssertions;
			bool									m_bGenerateAllDifferent;
			bool									m_bGenerateForceSingleType;
			bool									m_bGenerateTimeStamp;
			bool									m_bGenerateComments;
			bool									m_bGenerateUseMultipleDefault;
			bool									m_bGenerateInchiMapping;
	};


	
}

#endif //!defined(OWL_GENERATE_OPTIONS_H)