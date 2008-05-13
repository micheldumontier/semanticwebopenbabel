#if !defined(OWL_IMPORT_H)
#define OWL_IMPORT_H

#include <string>
#include <sstream>

namespace Owl
{
	class Import
	{
		public:
        
			Import();
			Import(const std::string& refPath);
			
		public:
        
			const std::string&		GetPath() const;
			void					Print(std::ostream* pOutputStream);

		private:
        
			std::string				m_sPath;
	};
}

#endif //!defined(OWL_IMPORT_H)