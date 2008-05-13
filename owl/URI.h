#if !defined(OWL_URI_H)
#define OWL_URI_H

#include <string>
#include <sstream>

namespace Owl
{
    class URI
	{
		public:
        
			URI();
			URI(const std::string& refPrefix, const std::string& refPath);
			URI(const std::string& refPrefix, const std::string& refPath, bool bSpecial);
			
		public:
        
			const std::string&		GetPrefix() const;
			const std::string&		GetPath() const;

			void					PrintEntity(std::ostream* pOutputStream);
			void					PrintURI(std::ostream* pOutputStream);

		private:
        
			std::string				m_sPath;
			std::string				m_sPrefix;
			bool					m_bSpecial;
	};
}

#endif //!defined(OWL_URI_H)