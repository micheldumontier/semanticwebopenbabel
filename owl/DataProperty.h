#if !defined(OWL_DATAPROPERTY_H)
#define OWL_DATAPROPERTY_H

#include <string>
#include <sstream>
#include <vector>
#include <set>

#include "DataPropertyOption.h"
#include "Class.h"

#include <openbabel/mol.h>

namespace Owl
{
	class DataProperty
	{

		public:
			DataProperty();
			DataProperty(const std::string& refPrefix, const std::string& refName, const std::string& refType);
            
			~DataProperty();
			
		public:
        
			const std::string&							GetPrefix() const;
			const std::string&							GetName() const;
			const std::string&							GetType() const;

			bool										IsDefault() const;

			void										AddOption(const std::string& refPrefix, const std::string& refName);

			void										Print(std::ostream* pOutputStream);
			void										RetrieveProperty(std::set<std::string>& refDataProperties);

			
			bool										SatisfyDirectDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refTo, 
																	std::vector<Class*>& vecSatisfiedClasses, std::string& refValue);
																	
			bool										Satisfy(OpenBabel::OBMol* pMolecule, const std::string& refTo, 
																	std::vector<Class*>& vecSatisfiedClasses, std::string& refValue);
																	
			bool										Satisfy(OpenBabel::OBAtom* pAtom, const std::string& refTo, 
																	std::vector<Class*>& vecSatisfiedClasses, std::string& refValue);
																	
			bool										Satisfy(OpenBabel::OBBond* pBond, const std::string& refTo, 
																	std::vector<Class*>& vecSatisfiedClasses, std::string& refValue);
																	
			bool										Satisfy(OpenBabel::OBRing* pRing, const std::string& refTo, 
																	std::vector<Class*>& vecSatisfiedClasses, std::string& refValue);

		private:

			bool										SatisfyDescriptor(OpenBabel::OBMol* pMolecule, const std::string& refTo, std::string& refValue);


			bool										SatisfyCommon(std::vector<Class*>& vecSatisfiedClasses, int& iPosition);

			void										ConvertInt(int iValue, std::string& refValue);
			void										ConvertBool(bool bValue, std::string& refValue);
			void										ConvertDouble(double dValue, std::string& refValue);

		private:
        
			std::string									m_sPrefix;
			std::string									m_sName;
			std::string									m_sType;

			std::vector<DataPropertyOption*>			m_vecOptions;
	};
}

#endif //!defined(OWL_DATAPROPERTY_H)