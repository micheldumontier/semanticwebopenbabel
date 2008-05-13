#if !defined(OWL_DATAPROPERTYOPTION_H)
#define OWL_DATAPROPERTYOPTION_H

#include <string>
#include <sstream>

namespace Owl
{
	class DataPropertyOption
	{
		public:
        
			DataPropertyOption();
			DataPropertyOption(const std::string& refPrefix, const std::string& refName);
			
		public:
        
			const std::string&							GetPrefix() const;
			const std::string&							GetName() const;

			void										Print(std::ostream* pOutputStream, int iIdent = 1);

		private:
        
			std::string									m_sPrefix;
			std::string									m_sName;
	};
}

#endif //!defined(OWL_DATAPROPERTYOPTION_H)