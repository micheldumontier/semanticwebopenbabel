#include "Configuration.h"
#include "Class.h"

#include <sstream>
#include <set>

#include <strings.h>

#include "RelationMolecule.h"
#include "RelationAtom.h"
#include "RelationBond.h"
#include "RelationRing.h"
#include "RelationDescriptor.h"

namespace Owl
{
	//--
	Configuration::Configuration() :
		m_sBase(""), 
		m_sBaseOntology(""),
		m_pOutputStream(NULL),
		m_pMolecule(NULL)
	{
		m_mapXmlBlocks["OWLPlugin"]				= XmlBlockPlugin;
		m_mapXmlBlocks["Generate"]				= XmlBlockGenerate;
		m_mapXmlBlocks["URIs"]					= XmlBlockURIs;
		m_mapXmlBlocks["Imports"]				= XmlBlockImports;
		m_mapXmlBlocks["Classes"]				= XmlBlockClasses;
		m_mapXmlBlocks["DataProperties"]		= XmlBlockDataProperties;
		m_mapXmlBlocks["ObjectProperties"]		= XmlBlockObjectProperties;
	}


    //--
    Configuration::~Configuration()
    {
        Reset();
    }



	//--
	// [rad] load configuration from a file
	bool Configuration::Load(const std::string& refConfFile)
	{
		TiXmlDocument* pXMLDocument = new TiXmlDocument(refConfFile);

		// [rad] check if document was created
		if(!pXMLDocument)
		{
			return(false);
		}

		// [rad] check if document was loaded
		if(!pXMLDocument->LoadFile())
		{
			delete(pXMLDocument);
			return(false);
		}

		// [rad] get root node
		TiXmlElement* pNodeOWLPlugin = pXMLDocument->FirstChildElement("OWLPlugin");
		if(!pNodeOWLPlugin)
		{
			pXMLDocument->Clear();
			delete(pXMLDocument);

			return(false);
		}



		// [rad] get generate section
		TiXmlElement* pNodeGenerate = pNodeOWLPlugin->FirstChildElement("Generate");
		if(pNodeGenerate)
		{
			TiXmlElement* pNodeGenerateOption;
			std::string sAttribute;

			// [rad] Whether to generate Hydrogens
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("Hydrogens");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateHydrogens = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

			// [rad] Whether to generate assertions
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("Assertions");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateAssertions = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

			// [rad] Whether to generate all-different
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("AllDifferent");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateAllDifferent = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

			// [rad] If instance is found to be satisfiable with multiple options, declare all, or only one?
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("ForceSingleType");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateForceSingleType = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

			// [rad] If none of the conditions has been satisfied and default is found, use it?
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("UseMultipleDefault");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateUseMultipleDefault = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

			// [rad] Whether to timestamp the ontology
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("TimeStamp");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateTimeStamp = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

			// [rad] Whether to generate comments
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("Comments");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateComments = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}
			
			// [rad] Whether to generate inchi mapping
			pNodeGenerateOption = pNodeGenerate->FirstChildElement("InchiMapping");
			if(pNodeGenerateOption)
			{
				// [rad] check if we have 
				if(pNodeGenerateOption->Attribute("enabled"))
				{
					m_objOptions.m_bGenerateInchiMapping = IsAttributeEnabled(pNodeGenerateOption->Attribute("enabled"));
				}
			}

		}



		// [rad] get base section
		TiXmlElement* pNodeBase = pNodeOWLPlugin->FirstChildElement("Base");
		if(pNodeBase)
		{
			// [rad] check if we have path attribute
			if(pNodeBase->Attribute("path"))
			{
				m_sBase = pNodeBase->Attribute("path");
			}
		}



		// [rad] get URI section
		TiXmlElement* pNodeURIs = pNodeOWLPlugin->FirstChildElement("URIs");
		if(pNodeURIs)
		{
			TiXmlElement* pNodeURI = pNodeURIs->FirstChildElement("URI");
			while(pNodeURI)
			{
				// [rad] check if we have both prefix and path
				if(pNodeURI->Attribute("prefix") && pNodeURI->Attribute("path"))
				{
					// [rad] check if we need special treatment
					if(pNodeURI->Attribute("special"))
					{
						m_vecURIs.push_back(new URI(pNodeURI->Attribute("prefix"), 
											pNodeURI->Attribute("path"), AttributeToBool(pNodeURI->Attribute("special"))));
					}
					else
					{
						// [rad] store URI entry
						m_vecURIs.push_back(new URI(pNodeURI->Attribute("prefix"), pNodeURI->Attribute("path")));
					}
				}

				// [rad] grab next URI
				pNodeURI = pNodeURI->NextSiblingElement("URI");
			}
		}



		// [rad] get imports section
		TiXmlElement* pNodeImports = pNodeOWLPlugin->FirstChildElement("Imports");
		if(pNodeImports)
		{
			TiXmlElement* pNodeImport = pNodeImports->FirstChildElement("Import");
			while(pNodeImport)
			{
				// [rad] check if we have path
				if(pNodeImport->Attribute("path"))
				{
                    m_vecImports.push_back(new Import(pNodeImport->Attribute("path")));
				}

				// [rad] grab next import
				pNodeImport = pNodeImport->NextSiblingElement("Import");
			}
		}



		// [rad] get classes section
		TiXmlElement* pNodeClasses = pNodeOWLPlugin->FirstChildElement("Classes");
		if(pNodeClasses)
		{
			TiXmlElement* pNodeClassResource = pNodeClasses->FirstChildElement("ClassResource");
			while(pNodeClassResource)
			{
				// [rad] class-resource must have a name
				if(pNodeClassResource->Attribute("name"))
				{
					// [rad] check if we have auxiliary information available
					if(pNodeClassResource->Attribute("auxiliary"))
					{
						// [rad] add this class-resource
						m_vecClassResources.push_back(new ClassResource(pNodeClassResource->Attribute("name"), 
																		pNodeClassResource->Attribute("auxiliary")));
					}
					else
					{
						// [rad] add this class-resource
						m_vecClassResources.push_back(new ClassResource(pNodeClassResource->Attribute("name")));
					}
				
					// [rad] go through classes assigned to this class-resource
					TiXmlElement* pNodeClass = pNodeClassResource->FirstChildElement("Class");
					while(pNodeClass)
					{
						// [rad] must have both prefix and name attributes
						if(pNodeClass->Attribute("name") && pNodeClass->Attribute("prefix"))
						{
							// [rad] find last class-resource
                            ClassResource* objClassResource = m_vecClassResources.back();

							// [rad] Add Class to it
							objClassResource->AddClass(pNodeClass->Attribute("prefix"), pNodeClass->Attribute("name"));


							// [rad] Process all subnodes - these will be either condition, union or intersection nodes
							for(TiXmlNode* pNodeRestriction = pNodeClass->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
							{
								// [rad] must be an element
								if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

								//TiXmlElement* pNodeRestrictionElement = const_cast<TiXmlElement*>(pNodeRestriction);
								TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();

								if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
								{
									// [rad] this is a condition child node

									// [rad] Retrieve last ClassResource
                                    ClassResource* objClassResource = m_vecClassResources.back();

									// [rad] must have on and value, operation is not necessary
									if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
									{
										// [rad] if we have operation, use that
										if(pNodeRestrictionElement->Attribute("operation"))
										{
											// [rad] Add condition to it
											objClassResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
										}
										else
										{
											// [rad] Add condition to it
											objClassResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
										}
									}

								}
								else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
								{
									// [rad] go inside this union
									ProcessClassUnion(pNodeRestrictionElement);

								}
								else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
								{
									// [rad] go inside this intersection
									ProcessClassIntersection(pNodeRestrictionElement);
								}
							}
						}

						// [rad] iterate to next class
						pNodeClass = pNodeClass->NextSiblingElement("Class");
					}

					pNodeClass = NULL;
				}

				// [rad] grab next class-resource
				pNodeClassResource = pNodeClassResource->NextSiblingElement("ClassResource");
			}

			pNodeClassResource = NULL;
		}

		pNodeClasses = NULL;

		

		// [rad] get data properties section
		TiXmlElement* pNodeDataProperties = pNodeOWLPlugin->FirstChildElement("DataProperties");
		if(pNodeDataProperties)
		{
			TiXmlElement* pNodeDataPropertyResource = pNodeDataProperties->FirstChildElement("DataPropertyResource");
			while(pNodeDataPropertyResource)
			{
				// [rad] data-property-resource must have to and from attributes
				if(pNodeDataPropertyResource->Attribute("to") && pNodeDataPropertyResource->Attribute("from"))
				{
					// [rad] store this data-property-resource
				    m_vecDataPropertyResources.push_back(new DataPropertyResource(pNodeDataPropertyResource->Attribute("from"), pNodeDataPropertyResource->Attribute("to")));
					
					// [rad] go through data-properties assigned to this resource
					TiXmlElement* pNodeDataProperty = pNodeDataPropertyResource->FirstChildElement("DataProperty");
					while(pNodeDataProperty)
					{
						// [rad] data-property must have prefix, name and type attributes
						if(pNodeDataProperty->Attribute("prefix") && pNodeDataProperty->Attribute("type") && pNodeDataProperty->Attribute("name"))
						{
							// [rad] Retrieve last data-property-resource
                            DataPropertyResource* objDataPropertyResource = m_vecDataPropertyResources.back();

							// [rad] Add DataProperty to it
							objDataPropertyResource->AddDataProperty(pNodeDataProperty->Attribute("prefix"), pNodeDataProperty->Attribute("name"), pNodeDataProperty->Attribute("type"));

							// [rad] we need to process all options present
							TiXmlElement* pNodeOption = pNodeDataProperty->FirstChildElement("Option");
							while(pNodeOption)
							{
								// [rad] option must have name and prefix attributes
								if(pNodeOption->Attribute("name") && pNodeOption->Attribute("prefix"))
								{
									objDataPropertyResource->AddOption(pNodeOption->Attribute("prefix"), pNodeOption->Attribute("name"));
								}

								// [rad] iterate to next option
								pNodeOption = pNodeOption->NextSiblingElement("Option");
							}

							pNodeOption = NULL;

						}

						// [rad] iterate to next data-property
						pNodeDataProperty = pNodeDataProperty->NextSiblingElement("DataProperty");
					}

					pNodeDataProperty = NULL;

				}

				// [rad] iterate to next data-property-resouce
				pNodeDataPropertyResource = pNodeDataPropertyResource->NextSiblingElement("DataPropertyResource");
			}

			pNodeDataPropertyResource = NULL;
		}

		pNodeClasses = NULL;




		// [rad] get object properties section
		TiXmlElement* pNodeObjectProperties = pNodeOWLPlugin->FirstChildElement("ObjectProperties");
		if(pNodeObjectProperties)
		{
			TiXmlElement* pNodeObjectPropertyResource = pNodeObjectProperties->FirstChildElement("ObjectPropertyResource");
			while(pNodeObjectPropertyResource)
			{
				// [rad] object property resource must have to and from attributes, constraint is not necessary
				if(pNodeObjectPropertyResource->Attribute("to") && pNodeObjectPropertyResource->Attribute("from"))
				{
					// [rad] add this object-property-resource
					if(pNodeObjectPropertyResource->Attribute("constraint"))
					{
						m_vecObjectPropertyResources.push_back(
													new ObjectPropertyResource(pNodeObjectPropertyResource->Attribute("from"), pNodeObjectPropertyResource->Attribute("to"), pNodeObjectPropertyResource->Attribute("constraint")));					
					}
					else
					{
                        m_vecObjectPropertyResources.push_back(
													new ObjectPropertyResource(pNodeObjectPropertyResource->Attribute("from"), pNodeObjectPropertyResource->Attribute("to")));					
					
					}

					// [rad] we need to iterate through object-properties in this object-property-resource
					TiXmlElement* pNodeObjectProperty = pNodeObjectPropertyResource->FirstChildElement("ObjectProperty");
					while(pNodeObjectProperty)
					{
						// [rad] object-property must have prefix and name, auxiliary is not necessary
						if(pNodeObjectProperty->Attribute("prefix") && pNodeObjectProperty->Attribute("name"))
						{
							// [rad] Retrieve last object-property-resource
						    ObjectPropertyResource* objObjectPropertyResource = m_vecObjectPropertyResources.back();

							if(pNodeObjectProperty->Attribute("auxiliary"))
							{
								// [rad] Add object-property
								objObjectPropertyResource->AddObjectProperty(pNodeObjectProperty->Attribute("prefix"), pNodeObjectProperty->Attribute("name"), pNodeObjectProperty->Attribute("auxiliary"));
							}
							else
							{
								// [rad] Add object-property
								objObjectPropertyResource->AddObjectProperty(pNodeObjectProperty->Attribute("prefix"), pNodeObjectProperty->Attribute("name"));
							}

							// [rad] add domain, if present
							TiXmlElement* pNodeDomain = pNodeObjectProperty->FirstChildElement("Domain");
							if(pNodeDomain)
							{
								// [rad] domain must have prefix and name attributes
								if(pNodeDomain->Attribute("prefix") && pNodeDomain->Attribute("name"))
								{
									objObjectPropertyResource->AddDomain(pNodeDomain->Attribute("prefix"), pNodeDomain->Attribute("name"));
								}

								// [rad] process domain subnodes - these will be either condition, union or intersection nodes
								for(TiXmlNode* pNodeRestriction = pNodeDomain->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
								{
									// [rad] must be an element
									if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

									TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();

									if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
									{
										// [rad] this is a condition child node

										// [rad] must have on and value, operation is not necessary
										if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
										{
											// [rad] if we have operation, use that
											if(pNodeRestrictionElement->Attribute("operation"))
											{
												// [rad] Add condition to it
												objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
											}
											else
											{
												// [rad] Add condition to it
												objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
											}
										}
									}
									else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
									{
										// [rad] go inside this union
										ProcessObjectPropertyUnion(pNodeRestrictionElement);
									}
									else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
									{
										// [rad] go inside this intersection
										ProcessObjectPropertyIntersection(pNodeRestrictionElement);
									}
								}
							}

							pNodeDomain = NULL;

							// [rad] add range, if present
							TiXmlElement* pNodeRange = pNodeObjectProperty->FirstChildElement("Range");
							if(pNodeRange)
							{
								// [rad] range must have prefix and name attributes
								if(pNodeRange->Attribute("prefix") && pNodeRange->Attribute("name"))
								{
									objObjectPropertyResource->AddRange(pNodeRange->Attribute("prefix"), pNodeRange->Attribute("name"));
								}

								// [rad] process range subnodes - these will be either condition, union or intersection nodes
								for(TiXmlNode* pNodeRestriction = pNodeRange->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
								{
									// [rad] must be an element
									if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

									TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();

									if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
									{
										// [rad] this is a condition child node

										// [rad] must have on and value, operation is not necessary
										if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
										{
											// [rad] if we have operation, use that
											if(pNodeRestrictionElement->Attribute("operation"))
											{
												// [rad] Add condition to it
												objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
											}
											else
											{
												// [rad] Add condition to it
												objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
											}
										}
									}
									else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
									{
										// [rad] go inside this union
										ProcessObjectPropertyUnion(pNodeRestrictionElement);
									}
									else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
									{
										// [rad] go inside this intersection
										ProcessObjectPropertyIntersection(pNodeRestrictionElement);
									}
								}
							}

							pNodeRange = NULL;

						}

						// [rad] iterate to next object-property
						pNodeObjectProperty = pNodeObjectProperty->NextSiblingElement("ObjectProperty");
					}

					pNodeObjectProperty = NULL;
				}

				// [rad] iterate to next object-property-resource
				pNodeObjectPropertyResource = pNodeObjectPropertyResource->NextSiblingElement("ObjectPropertyResource");
			}

			pNodeObjectPropertyResource = NULL;
		}

		pNodeObjectProperties = NULL;


		// [rad] clean up the document
		pXMLDocument->Clear();
		delete(pXMLDocument);

		return(true);
	}




	//--
	// [rad] Helper method to recurse into Class Union restrictions
	void Configuration::ProcessClassUnion(TiXmlElement* pNodeUnion)
	{
		// [rad] Retrieve last ClassResource
        ClassResource* objClassResource = m_vecClassResources.back();

		// [rad] Union has started
		objClassResource->UnionStart();

		// [rad] we need to process all children, and recurse deeper, if necessary
		for(TiXmlNode* pNodeRestriction = pNodeUnion->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
		{
			// [rad] must be an element
			if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

			TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();
			if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
			{
				// [rad] must have on and value, operation is not necessary
				if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
				{
					// [rad] if we have operation, use that
					if(pNodeRestrictionElement->Attribute("operation"))
					{
						// [rad] Add condition to it
						objClassResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
					}
					else
					{
						// [rad] Add condition to it
						objClassResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
					}
				}
			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
			{
				// [rad] go inside this union, recursively
				ProcessClassUnion(pNodeRestrictionElement);

			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
			{
				// [rad] go inside this intersection, recursively
				ProcessClassIntersection(pNodeRestrictionElement);
			}
		}

		// [rad] Union has ended
		objClassResource->UnionEnd();
	}




	//--
	// [rad] helper method to recurse into Class Intersection restrictions
	void Configuration::ProcessClassIntersection(TiXmlElement* pNodeIntersection)
	{
		// [rad] Retrieve last ClassResource
        ClassResource* objClassResource = m_vecClassResources.back();

		// [rad] Intersection has started
		objClassResource->IntersectionStart();

		// [rad] we need to process all children, and recurse deeper, if necessary
		for(TiXmlNode* pNodeRestriction = pNodeIntersection->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
		{
			// [rad] must be an element
			if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

			TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();
			if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
			{
				// [rad] must have on and value, operation is not necessary
				if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
				{
					// [rad] if we have operation, use that
					if(pNodeRestrictionElement->Attribute("operation"))
					{
						// [rad] Add condition to it
						objClassResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
					}
					else
					{
						// [rad] Add condition to it
						objClassResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
					}
				}
			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
			{
				// [rad] go inside this union, recursively
				ProcessClassUnion(pNodeRestrictionElement);

			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
			{
				// [rad] go inside this intersection, recursively
				ProcessClassIntersection(pNodeRestrictionElement);
			}
		}

		// [rad] Union has ended
		objClassResource->IntersectionEnd();

	}




	//--
	// [rad] helper method to recurse into Object Property Union restrictions
	void Configuration::ProcessObjectPropertyUnion(TiXmlElement* pNodeUnion)
	{
		// [rad] Retrieve last object-property-resource
        ObjectPropertyResource* objObjectPropertyResource = m_vecObjectPropertyResources.back();

		// [rad] Union has started
		objObjectPropertyResource->UnionStart();

		// [rad] we need to process all children, and recurse deeper, if necessary
		for(TiXmlNode* pNodeRestriction = pNodeUnion->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
		{
			// [rad] must be an element
			if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

			TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();
			if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
			{
				// [rad] must have on and value, operation is not necessary
				if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
				{
					// [rad] if we have operation, use that
					if(pNodeRestrictionElement->Attribute("operation"))
					{
						// [rad] Add condition to it
						objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
					}
					else
					{
						// [rad] Add condition to it
						objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
					}
				}
			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
			{
				// [rad] go inside this union, recursively
				ProcessObjectPropertyUnion(pNodeRestrictionElement);

			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
			{
				// [rad] go inside this intersection, recursively
				ProcessObjectPropertyIntersection(pNodeRestrictionElement);
			}
		}

		// [rad] Union has ended
		objObjectPropertyResource->UnionEnd();
	}




	//--
	// [rad] helper method to recurse into Object Property Intersection restrictions
	void Configuration::ProcessObjectPropertyIntersection(TiXmlElement* pNodeIntersection)
	{
		// [rad] Retrieve last object-property-resource
        ObjectPropertyResource* objObjectPropertyResource = m_vecObjectPropertyResources.back();

		// [rad] Union has started
		objObjectPropertyResource->IntersectionStart();

		// [rad] we need to process all children, and recurse deeper, if necessary
		for(TiXmlNode* pNodeRestriction = pNodeIntersection->FirstChild(); pNodeRestriction; pNodeRestriction = pNodeRestriction->NextSibling())
		{
			// [rad] must be an element
			if(TiXmlNode::ELEMENT != pNodeRestriction->Type()) continue;

			TiXmlElement* pNodeRestrictionElement = pNodeRestriction->ToElement();
			if(!pNodeRestrictionElement->ValueStr().compare("Condition"))
			{
				// [rad] must have on and value, operation is not necessary
				if(pNodeRestrictionElement->Attribute("on") && pNodeRestrictionElement->Attribute("value"))
				{
					// [rad] if we have operation, use that
					if(pNodeRestrictionElement->Attribute("operation"))
					{
						// [rad] Add condition to it
						objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"), pNodeRestrictionElement->Attribute("operation"));
					}
					else
					{
						// [rad] Add condition to it
						objObjectPropertyResource->AddCondition(pNodeRestrictionElement->Attribute("on"), pNodeRestrictionElement->Attribute("value"));
					}
				}
			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Union"))
			{
				// [rad] go inside this union, recursively
				ProcessObjectPropertyUnion(pNodeRestrictionElement);

			}
			else if(!pNodeRestrictionElement->ValueStr().compare("Intersection"))
			{
				// [rad] go inside this intersection, recursively
				ProcessObjectPropertyIntersection(pNodeRestrictionElement);
			}
		}

		// [rad] Union has ended
		objObjectPropertyResource->IntersectionEnd();

	}




	/*
	//--
	// [rad] load configuration from a file - not used, is using libxml sax
	bool Configuration::Load(const std::string& refConfFile)
	{
		// [rad] Create an xml reader
		xmlTextReader* pXmlReader = xmlReaderForFile(refConfFile.c_str(), NULL, 0);

		// [rad] Failed loading - configuration file does not exist?
		if(!pXmlReader) return(false);

		int iNodeType;		
		const xmlChar* pNodeNameBuf;

		// [rad] Initial parsing state is undefined
		XmlBlock enCurrentState = XmlBlockNone;

		// [rad] xml blocks iterator
		std::map<std::string, XmlBlock>::iterator iter_XmlBlocks;

		// [rad] Read the first xml node
		int iResult = xmlTextReaderRead(pXmlReader);

		// [rad] Iterate over each xml node present in the configuration file
		while(iResult)
		{
			// [rad] take care of error, if happened
			if(-1 == iResult)
			{
				// [rad] Free resources used up by reader
				xmlFreeTextReader(pXmlReader);	

				// [rad] reset configuration
				Reset();

				// [rad] return failed status
				return(false);
			}

			// [rad] Process node - get node name and type
			pNodeNameBuf = xmlTextReaderConstLocalName(pXmlReader);
			iNodeType = xmlTextReaderNodeType(pXmlReader);
			
			if(XML_READER_TYPE_ELEMENT == iNodeType)
			{
				// [rad] If we are dealing with an element

				// [rad] see if this is a valid block
				iter_XmlBlocks = m_mapXmlBlocks.find((const char*) pNodeNameBuf);
				if(m_mapXmlBlocks.end() != iter_XmlBlocks)
				{
					// [rad] valid block, update current state
					enCurrentState = iter_XmlBlocks->second;
				}
				else
				{
					// [rad] base is a special case
					if(!strcmp((const char*) pNodeNameBuf, "Base"))
					{
						// [rad] Retrieve value of "path" attribute
						const xmlChar* pNodeAttributePath = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "path");

						// [rad] if we do have a path attribute
						if(pNodeAttributePath)
						{
							m_sBase = (const char*) pNodeAttributePath;
						}
					}
					else
					{
						switch(enCurrentState)
						{
							case XmlBlockGenerate:
								{
									// [rad] we are in generate block

									// [rad] Retrieve the value of attribute "enabled"			
									const xmlChar* pNodeAttributeBuf = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "enabled");

									// [rad] if we have such attribute - use it
									if(pNodeAttributeBuf)
									{
										if(!strcmp((const char*) pNodeNameBuf, "Hydrogens"))
										{
											m_objOptions.m_bGenerateHydrogens = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "Assertions"))
										{
											m_objOptions.m_bGenerateAssertions = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "AllDifferent"))
										{
											m_objOptions.m_bGenerateAllDifferent = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "ForceSingleType"))
										{
											m_objOptions.m_bGenerateForceSingleType = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "TimeStamp"))
										{
											m_objOptions.m_bGenerateTimeStamp = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "Comments"))
										{
											m_objOptions.m_bGenerateComments = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "UseMultipleDefault"))
										{
											m_objOptions.m_bGenerateUseMultipleDefault = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
										else if(!strcmp((const char*) pNodeNameBuf, "InchiMapping"))
										{
											m_objOptions.m_bGenerateInchiMapping = IsAttributeEnabled((const char*) pNodeAttributeBuf);
										}
									}
								}
								break;



							case XmlBlockURIs:
								{
									// [rad] we allow only URI blocks inside URIs
									if(!strcmp((const char*) pNodeNameBuf, "URI"))
									{
										// [rad] Retrieve values of "prefix" and "path" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributePath = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "path");

										// [rad] we need values of both prefix and path
										if(pNodeAttributePath && pNodeAttributePrefix)
										{
											// [rad] store this URI entry
											m_vecURIs.push_back(Ptr_URI(new URI((const char*) pNodeAttributePrefix, (const char*) pNodeAttributePath)));
										}
									}
								}
								break;



							case XmlBlockImports:
								{
									// [rad] we allow only Import blocks inside Imports
									if(!strcmp((const char*) pNodeNameBuf, "Import"))
									{
										// [rad] Retrieve the value of "path" attribute
										const xmlChar* pNodeAttributePath = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "path");

										// [rad] we need "path" attribute
										if(pNodeAttributePath)
										{
											// [rad] store import entry
											m_vecImports.push_back(Ptr_Import(new Import((const char*) pNodeAttributePath)));
										}
									}
								}
								break;



							case XmlBlockClasses:
								{
									// [rad] only class resources are allowed within classes 
									if(!strcmp((const char*) pNodeNameBuf, "ClassResource"))
									{
										// [rad] We are inside class resource
										enCurrentState = XmlBlockClassResource;

										// [rad] Retrieve values of "name" attribute
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");

										// [rad] we must have a name attribute
										if(pNodeAttributeName)
										{
											// [rad] we don't need to check if such class resource exists
											m_vecClassResources.push_back(Ptr_ClassResource(new ClassResource((const char*) pNodeAttributeName)));
										}
									}
								}
								break;



							case XmlBlockClassResource:
								{
									// [rad] only classes are allowed within class resources
									if(!strcmp((const char*) pNodeNameBuf, "Class"))
									{
										// [rad] we are now inside class
										enCurrentState = XmlBlockClass;

										// [rad] Retrieve values of "name" and "prefix" attributes
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");

										// [rad] we must have both attributes
										if(pNodeAttributeName && pNodeAttributePrefix)
										{
											// [rad] we don't need to check if such class exists
											
											// [rad] If we have ClassResources
											if(m_vecClassResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ClassResource objClassResource = m_vecClassResources.back();

												// [rad] Add Class to it
												objClassResource->AddClass((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

												// [rad] Decrement reference counter
												objClassResource.reset();
											}
										}
									}
								}
								break;



							case XmlBlockClass:
							case XmlBlockClassUnion:
							case XmlBlockClassIntersection:
								{
									if(!strcmp((const char*) pNodeNameBuf, "Class"))
									{
										// [rad] we are now inside class
										enCurrentState = XmlBlockClass;

										// [rad] Retrieve values of "name" and "prefix" attributes
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");

										// [rad] we must have both attributes
										if(pNodeAttributeName && pNodeAttributePrefix)
										{
											// [rad] we don't need to check if such class exists
											
											// [rad] If we have ClassResources
											if(m_vecClassResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ClassResource objClassResource = m_vecClassResources.back();

												// [rad] Add Class to it
												objClassResource->AddClass((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

												// [rad] Decrement reference counter
												objClassResource.reset();
											}
										}
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Condition"))
									{
										// [rad] Retrieve values of "on" and "value" attributes, "operation" is not necessary
										const xmlChar* pNodeAttributeOn = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "on");
										const xmlChar* pNodeAttributeValue = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "value");
										const xmlChar* pNodeAttributeOperator = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "operation");

										if(pNodeAttributeOn && pNodeAttributeValue)
										{
											// [rad] Retrieve last ClassResource
											Ptr_ClassResource objClassResource = m_vecClassResources.back();

											if(pNodeAttributeOperator)
											{
												// [rad] Add condition to it
												objClassResource->AddCondition((const char*) pNodeAttributeOn, 
																				(const char*) pNodeAttributeValue, (const char*) pNodeAttributeOperator);
											}
											else
											{
												// [rad] Add condition to it
												objClassResource->AddCondition((const char*) pNodeAttributeOn, (const char*) pNodeAttributeValue);
											}

											// [rad] Decrement reference counter
											objClassResource.reset();
										}
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Union"))
									{
										// [rad] we are now inside union
										enCurrentState = XmlBlockClassUnion;

										// [rad] Retrieve last ClassResource
										Ptr_ClassResource objClassResource = m_vecClassResources.back();

										// [rad] Add union to it
										objClassResource->UnionStart();

										// [rad] Decrement reference counter
										objClassResource.reset();
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Intersection"))
									{
										// [rad] we are now inside intersection
										enCurrentState = XmlBlockClassIntersection;

										// [rad] Retrieve last ClassResource
										Ptr_ClassResource objClassResource = m_vecClassResources.back();

										// [rad] Add intersection to it
										objClassResource->IntersectionStart();

										// [rad] Decrement reference counter
										objClassResource.reset();
									}
								}
								break;

							
							case XmlBlockDataProperties:
								{
									// [rad] only data property resources are allowed within datapaproperties 
									if(!strcmp((const char*) pNodeNameBuf, "DataPropertyResource"))
									{
										// [rad] We are inside dataproperty resource
										enCurrentState = XmlBlockDataPropertyResource;

										// [rad] Retrieve values of "from" and "to" attributes
										const xmlChar* pNodeAttributeTo = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "to");
										const xmlChar* pNodeAttributeFrom = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "from");

										// [rad] we must have both attributes
										if(pNodeAttributeTo && pNodeAttributeFrom)
										{
											// [rad] we don't need to check if such class resource exists
											//m_vecClassResources.push_back(Ptr_ClassResource(new ClassResource((const char*) pNodeAttributeName)));
											m_vecDataPropertyResources.push_back(Ptr_DataPropertyResource(new DataPropertyResource((const char*) pNodeAttributeFrom, (const char*) pNodeAttributeTo)));
										}
									}
								}
								break;

							case XmlBlockDataPropertyResource:
								{
									// [rad] only data properties are allowed within data property resources
									if(!strcmp((const char*) pNodeNameBuf, "DataProperty"))
									{
										// [rad] we are now inside data property
										enCurrentState = XmlBlockDataProperty;

										// [rad] Retrieve values of "name" "prefix" "type" attributes
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeType = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "type");

										// [rad] we must have both attributes
										if(pNodeAttributeName && pNodeAttributePrefix && pNodeAttributeType)
										{
											// [rad] we don't need to check if such class exists
											
											// [rad] If we have DataPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_DataPropertyResource objDataPropertyResource = m_vecDataPropertyResources.back();

												// [rad] Add DataProperty to it
												objDataPropertyResource->AddDataProperty((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName, (const char*) pNodeAttributeType);

												// [rad] Decrement reference counter
												objDataPropertyResource.reset();
											}
										}
									}
								}
								break;


							case XmlBlockDataProperty:
								{
									// [rad] only options are allowed inside data properties
									if(!strcmp((const char*) pNodeNameBuf, "Option"))
									{
										// [rad] Retrieve values of "name" "prefix" attributes
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										
										if(pNodeAttributeName && pNodeAttributePrefix)
										{
											// [rad] Retrieve last DataProperty resource
											Ptr_DataPropertyResource objDataPropertyResource = m_vecDataPropertyResources.back();

											// [rad] Add option to it
											objDataPropertyResource->AddOption((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

											// [rad] Decrement reference counter
											objDataPropertyResource.reset();
										}
									}

								}
								break;


							case XmlBlockObjectProperties:
								{
									// [rad] only object property resources are allowed within object properties 
									if(!strcmp((const char*) pNodeNameBuf, "ObjectPropertyResource"))
									{
										// [rad] We are inside  object property resource
										enCurrentState = XmlBlockObjectPropertyResource;

										// [rad] Retrieve values of "from" and "to" attributes
										const xmlChar* pNodeAttributeTo = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "to");
										const xmlChar* pNodeAttributeFrom = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "from");
										const xmlChar* pNodeAttributeConstraint = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "constraint");

										// [rad] we must have both attributes
										if(pNodeAttributeTo && pNodeAttributeFrom)
										{
											// [rad] we don't need to check if such class resource exists

											if(!pNodeAttributeConstraint)
											{
												m_vecObjectPropertyResources.push_back(Ptr_ObjectPropertyResource(
													new ObjectPropertyResource((const char*) pNodeAttributeFrom, (const char*) pNodeAttributeTo)));
											}
											else
											{
												m_vecObjectPropertyResources.push_back(Ptr_ObjectPropertyResource(
													new ObjectPropertyResource((const char*) pNodeAttributeFrom, (const char*) pNodeAttributeTo,
													(const char*) pNodeAttributeConstraint)));
											}
										}
									}
								}
								break;


							case XmlBlockObjectPropertyResource:
								{
									// [rad] only object properties are allowed within object property resources
									if(!strcmp((const char*) pNodeNameBuf, "ObjectProperty"))
									{
										// [rad] we are inside object property
										enCurrentState = XmlBlockObjectProperty;

										// [rad] Retrieve values of "prefix" and "name" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");
										const xmlChar* pNodeAttributeAuxiliary = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "auxiliary");

										if(pNodeAttributePrefix && pNodeAttributeName)
										{
											// [rad] If we have ObjectPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												if(!pNodeAttributeAuxiliary)
												{
													// [rad] Add DataProperty to it
													objObjectPropertyResource->AddObjectProperty((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);
												}
												else
												{
													// [rad] Add DataProperty to it
													objObjectPropertyResource->AddObjectProperty((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName, (const char*) pNodeAttributeAuxiliary);
												}

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}

										}
									}
								}
								break;


							case XmlBlockObjectProperty:
								{
									if(!strcmp((const char*) pNodeNameBuf, "ObjectProperty"))
									{
										// [rad] we are inside object property
										enCurrentState = XmlBlockObjectProperty;

										// [rad] Retrieve values of "prefix" and "name" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");
										const xmlChar* pNodeAttributeAuxiliary = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "auxiliary");

										if(pNodeAttributePrefix && pNodeAttributeName)
										{
											// [rad] If we have ObjectPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												if(!pNodeAttributeAuxiliary)
												{
													// [rad] Add DataProperty to it
													objObjectPropertyResource->AddObjectProperty((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);
												}
												else
												{
													// [rad] Add DataProperty to it
													objObjectPropertyResource->AddObjectProperty((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName, (const char*) pNodeAttributeAuxiliary);
												}

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}

										}
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Domain"))
									{
										// [rad] we are inside domain
										enCurrentState = XmlBlockObjectPropertyDomain;

										// [rad] Retrieve values of "prefix" and "name" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");

										if(pNodeAttributePrefix && pNodeAttributeName)
										{
											// [rad] If we have ObjectPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												// [rad] Add DataProperty to it
												objObjectPropertyResource->AddDomain((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}
										}
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Range"))
									{
										// [rad] we are inside domain
										enCurrentState = XmlBlockObjectPropertyRange;

										// [rad] Retrieve values of "prefix" and "name" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");

										if(pNodeAttributePrefix && pNodeAttributeName)
										{
											// [rad] If we have ObjectPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												// [rad] Add DataProperty to it
												objObjectPropertyResource->AddRange((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}
										}
									}
								}
								break;


							case XmlBlockObjectPropertyDomain:
							case XmlBlockObjectPropertyRange:
							case XmlBlockObjectPropertyUnion:
							case XmlBlockObjectPropertyIntersection:
								{
									// [rad] inside domain/range we have 3 possible statements : Condition, Union and Intersection
									if(!strcmp((const char*) pNodeNameBuf, "Condition"))
									{
										// [rad] Retrieve values of "on" and "value" attributes, "operation" is not necessary
										const xmlChar* pNodeAttributeOn = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "on");
										const xmlChar* pNodeAttributeValue = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "value");
										const xmlChar* pNodeAttributeOperator = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "operation");

										if(pNodeAttributeOn && pNodeAttributeValue)
										{
											if(pNodeAttributeOperator)
											{
												// [rad] Retrieve last ObjectPropertyResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												// [rad] Add condition to it
												objObjectPropertyResource->AddCondition((const char*) pNodeAttributeOn, 
																				(const char*) pNodeAttributeValue, (const char*) pNodeAttributeOperator);

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}
											else
											{	
												// [rad] Retrieve last ObjectPropertyResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												// [rad] Add condition to it
												objObjectPropertyResource->AddCondition((const char*) pNodeAttributeOn, (const char*) pNodeAttributeValue);

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}	
										}
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Union"))
									{
										// [rad] we are now inside union
										enCurrentState = XmlBlockObjectPropertyUnion;

										// [rad] Retrieve last ClassResource
										Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

										// [rad] Add union to it
										objObjectPropertyResource->UnionStart();

										// [rad] Decrement reference counter
										objObjectPropertyResource.reset();
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Intersection"))
									{
										// [rad] we are now inside union
										enCurrentState = XmlBlockObjectPropertyIntersection;

										// [rad] Retrieve last ClassResource
										Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

										// [rad] Add union to it
										objObjectPropertyResource->UnionStart();

										// [rad] Decrement reference counter
										objObjectPropertyResource.reset();
									}
									// [rad] only domain and range are allowed within object property
									if(!strcmp((const char*) pNodeNameBuf, "Domain"))
									{
										// [rad] we are inside domain
										enCurrentState = XmlBlockObjectPropertyDomain;

										// [rad] Retrieve values of "prefix" and "name" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");

										if(pNodeAttributePrefix && pNodeAttributeName)
										{
											// [rad] If we have ObjectPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												// [rad] Add DataProperty to it
												objObjectPropertyResource->AddDomain((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}
										}
									}
									else if(!strcmp((const char*) pNodeNameBuf, "Range"))
									{
										// [rad] we are inside domain
										enCurrentState = XmlBlockObjectPropertyRange;

										// [rad] Retrieve values of "prefix" and "name" attributes
										const xmlChar* pNodeAttributePrefix = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "prefix");
										const xmlChar* pNodeAttributeName = xmlTextReaderGetAttribute(pXmlReader, (const xmlChar*) "name");

										if(pNodeAttributePrefix && pNodeAttributeName)
										{
											// [rad] If we have ObjectPropertyResources
											if(m_vecDataPropertyResources.size())
											{
												// [rad] Retrieve last ClassResource
												Ptr_ObjectPropertyResource objObjectPropertyResource = m_vecObjectPropertyResources.back();

												// [rad] Add DataProperty to it
												objObjectPropertyResource->AddRange((const char*) pNodeAttributePrefix, (const char*) pNodeAttributeName);

												// [rad] Decrement reference counter
												objObjectPropertyResource.reset();
											}
										}
									}
								}
								break;

						}
					}
				}
			}
			else if(XML_READER_TYPE_END_ELEMENT == iNodeType) 
			{
				// [rad] We are leaving node

				iter_XmlBlocks = m_mapXmlBlocks.find((const char*) pNodeNameBuf);
				if(m_mapXmlBlocks.end() != iter_XmlBlocks)
				{
					// [rad] valid block, update current state
					enCurrentState = XmlBlockPlugin;
				}
				else
				{
					if(!strcmp((const char*) pNodeNameBuf, "Class") && 
						(XmlBlockClass == enCurrentState || XmlBlockClassUnion == enCurrentState || XmlBlockClassIntersection == enCurrentState))
					{
						enCurrentState = XmlBlockClassResource;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "ObjectProperty") && 
						(XmlBlockObjectProperty == enCurrentState || XmlBlockObjectPropertyDomain == enCurrentState || XmlBlockObjectPropertyRange == enCurrentState))
					{
						enCurrentState = XmlBlockObjectPropertyResource;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "DataProperty") && (XmlBlockDataProperty == enCurrentState))
					{
						enCurrentState = XmlBlockDataPropertyResource;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "Domain") && (XmlBlockObjectPropertyDomain == enCurrentState))
					{
						enCurrentState = XmlBlockObjectProperty;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "Range") && (XmlBlockObjectPropertyRange == enCurrentState))
					{
						enCurrentState = XmlBlockObjectProperty;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "ClassResource") && (XmlBlockClass == enCurrentState || XmlBlockClassResource == enCurrentState))
					{
						enCurrentState = XmlBlockClasses;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "DataPropertyResource") && 
						(XmlBlockDataProperty == enCurrentState || XmlBlockDataPropertyResource == enCurrentState))
					{
						enCurrentState = XmlBlockDataProperties;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "ObjectPropertyResource") && 
						(XmlBlockObjectProperty == enCurrentState || XmlBlockObjectPropertyResource == enCurrentState))
					{
						enCurrentState = XmlBlockObjectProperties;
					}
					else if(!strcmp((const char*) pNodeNameBuf, "Union") && XmlBlockClassUnion == enCurrentState)
					{
						if(m_vecClassResources.size())
						{
							// [rad] Retrieve last ClassResource
							Ptr_ClassResource objClassResource = m_vecClassResources.back();

							objClassResource->UnionEnd();

							// [rad] Decrement reference counter
							objClassResource.reset();
						}
					}
					else if(!strcmp((const char*) pNodeNameBuf, "Intersection") && XmlBlockClassIntersection == enCurrentState)
					{
						if(m_vecClassResources.size())
						{
							// [rad] Retrieve last ClassResource
							Ptr_ClassResource objClassResource = m_vecClassResources.back();

							objClassResource->IntersectionEnd();

							// [rad] Decrement reference counter
							objClassResource.reset();
						}
					}
					else if(!strcmp((const char*) pNodeNameBuf, "Union") && XmlBlockObjectPropertyUnion == enCurrentState)
					{
						if(m_vecObjectPropertyResources.size())
						{
							// [rad] Retrieve last object property Resource
							Ptr_ObjectPropertyResource objPropertyResource = m_vecObjectPropertyResources.back();

							objPropertyResource->UnionEnd();

							// [rad] Decrement reference counter
							objPropertyResource.reset();
						}
					}
					else if(!strcmp((const char*) pNodeNameBuf, "Intersection") && XmlBlockObjectPropertyIntersection == enCurrentState)
					{
						if(m_vecObjectPropertyResources.size())
						{
							// [rad] Retrieve last ClassResource
							Ptr_ObjectPropertyResource objPropertyResource = m_vecObjectPropertyResources.back();

							objPropertyResource->IntersectionEnd();

							// [rad] Decrement reference counter
							objPropertyResource.reset();
						}
					}
				}
			}

			// [rad] Iterate to next node
			iResult = xmlTextReaderRead(pXmlReader);
		}

		// [rad] Free resources used up by reader
		xmlFreeTextReader(pXmlReader);	

		return(true);
	}
	*/


	
	//--
	// [rad] adds missing hydrogens - returns true if at least one hydrogen has been added, false otherwise
	bool Configuration::AddHydrogens()
	{
		// [rad] check flag - if we need to add
		if(!m_objOptions.m_bGenerateHydrogens)
		{
			// [rad] don't need to add
			return(false);
		}

		return(m_pMolecule->AddHydrogens());
	}





	//--
	// [rad] reset configuration
	void Configuration::Reset()
	{
		m_objOptions.Reset();

		m_sBase = "";
		m_sBaseOntology = "";
        
        
        // [rad] URIs
        std::vector<URI*>::iterator iter_uri = m_vecURIs.begin();
        while(iter_uri != m_vecURIs.end())
        {
            delete(*iter_uri);
            iter_uri = m_vecURIs.erase(iter_uri);
        }
        

        // [rad] Imports
        std::vector<Import*>::iterator iter_imports = m_vecImports.begin();
        while(iter_imports != m_vecImports.end())
        {
            delete(*iter_imports);
            iter_imports = m_vecImports.erase(iter_imports);
        }


        // [rad] ClassResource
        std::vector<ClassResource*>::iterator iter_class_rsc = m_vecClassResources.begin();
        while(iter_class_rsc != m_vecClassResources.end())
        {
            delete(*iter_class_rsc);
            iter_class_rsc = m_vecClassResources.erase(iter_class_rsc);
        }
        
        
        // [rad] DataPropertyResource
        std::vector<DataPropertyResource*>::iterator iter_data_rsc = m_vecDataPropertyResources.begin();
        while(iter_data_rsc != m_vecDataPropertyResources.end())
        {
            delete(*iter_data_rsc);
            iter_data_rsc = m_vecDataPropertyResources.erase(iter_data_rsc);
        }
        

        // [rad] ObjectPropertyResource
        std::vector<ObjectPropertyResource*>::iterator iter_obj_rsc = m_vecObjectPropertyResources.begin();
        while(iter_obj_rsc != m_vecObjectPropertyResources.end())
        {
            delete(*iter_obj_rsc);
            iter_obj_rsc = m_vecObjectPropertyResources.erase(iter_obj_rsc);
        }


        /*
		m_vecURIs.clear();
		m_vecImports.clear();
		m_vecClassResources.clear();
		m_vecDataPropertyResources.clear();
		m_vecObjectPropertyResources.clear();
        */

		m_mapIds.clear();
	}




	//--
	// [rad] Returns whether a given attribute value is set to true / yes / 1
	bool Configuration::IsAttributeEnabled(const char* pAttributeVal)
	{
		if(!pAttributeVal) return(false);
		
		// [rad] use of _strcmpi to make it iso compliant, possible problem when porting?
		//if(!_strcmpi(pAttributeVal, "true") || !_strcmpi(pAttributeVal, "1") || !_strcmpi(pAttributeVal, "yes")) return(true);
		
		if(!strcasecmp(pAttributeVal, "true") || !strcasecmp(pAttributeVal, "1") || !strcasecmp(pAttributeVal, "yes")) return(true);
		
		
		
		return(false);		
	}

	


	//--
	// [rad] Convert attribute to bool	
	bool Configuration::AttributeToBool(const std::string& refAttribute)
	{
		return(IsAttributeEnabled(refAttribute.c_str()));
	}



	//--
	// [rad] Convert attribute to an int
	int	Configuration::AttributeToInt(const std::string& refAttribute)
	{
		if(!refAttribute.length()) return(0);
	
		int iValue = 0;	
		std::istringstream ssConv;
		ssConv.str(refAttribute);
		ssConv >> iValue;
	
		return(iValue);
	}


	//--
	// [rad] Set output stream
	void Configuration::SetOutputStream(std::ostream* pOutputStream)
	{
		m_pOutputStream = pOutputStream;
	}


	//--
	// [rad] Set molecule
	void Configuration::SetMolecule(OpenBabel::OBMol* pMolecule)
	{
		m_pMolecule = pMolecule;
		
		// [rad] Save ontology base
		m_sBaseOntology = m_sBase + "Molecule-" + std::string(pMolecule->GetTitle()) + ".owl";
	}



	//--
	inline void	Configuration::WriteSpace()
	{
		*m_pOutputStream << std::endl << std::endl;
	}


	//--
	// [rad] Write entities
	void Configuration::WriteEntities()
	{
		// [rad] Write xml header
		*m_pOutputStream << "<?xml version=\"1.0\"?>" <<  std::endl; 

		WriteSpace();

		// [rad] Write entity block
		*m_pOutputStream << "<!DOCTYPE rdf:RDF [" << std::endl;

		// [rad] print all entities
		for(int i = 0; i < m_vecURIs.size(); i++) 
		{
			m_vecURIs[i]->PrintEntity(m_pOutputStream);
		}

		*m_pOutputStream << "]>" << std::endl;

		WriteSpace();	
	}



	//--
	// [rad] Write header
	void Configuration::WriteHeader()
	{
		*m_pOutputStream << "<rdf:RDF xmlns=\"" << m_sBaseOntology << "#\"" << std::endl;
		*m_pOutputStream << "         xml:base=\"" << m_sBaseOntology << "\"" << std::endl;

		for(int i = 0; i < m_vecURIs.size(); i++) 
		{
			m_vecURIs[i]->PrintURI(m_pOutputStream);
		}

		*m_pOutputStream << ">" << std::endl;

		WriteSpace();
	}



	//--
	// [rad] Write ontology
	void Configuration::WriteOntology()
	{
		// [rad] Writing ontology
		*m_pOutputStream << "<owl:Ontology rdf:about=\"\">" << std::endl;

		if(m_vecImports.size())
		{
			*m_pOutputStream << "    <owl:imports>" << std::endl;

			for(int i = 0; i < m_vecImports.size(); i++) 
			{
				m_vecImports[i]->Print(m_pOutputStream);
			}

			*m_pOutputStream << "    </owl:imports>" << std::endl;
		}


		// [rad] Get current date if we have to
		if(m_objOptions.m_bGenerateTimeStamp)
		{		
			time_t objDate;
			time(&objDate);
			std::string sTime = ctime(&objDate);

			// [rad] All this code is to remove "\n" from generated time string
			size_t objPos = sTime.find_first_of("\n");
			if(std::string::npos != objPos)
			{
				sTime.erase(objPos, 1);
			}

			*m_pOutputStream << "    <rdfs:comment rdf:datatype=\"&xsd;string\">" << "Generated " <<  sTime << "</rdfs:comment>" << std::endl;
		}

		*m_pOutputStream << "</owl:Ontology>" << std::endl;
		
		WriteSpace();
	}



	//--
	// [rad] Write footer
	void Configuration::WriteFooter()
	{
		WriteSpace();

		*m_pOutputStream << "</rdf:RDF>" <<  std::endl; 
	}



	//--
	// [rad] Adds a comment block
	void Configuration::WriteCommentBlock(const std::string& refComment)
	{
		if(m_objOptions.m_bGenerateComments)
		{
			*m_pOutputStream << "<!--" << std::endl;
			*m_pOutputStream << "///////////////////////////////////////////////////////////////////////////" << std::endl;
			*m_pOutputStream << "//" << std::endl;
			*m_pOutputStream << "//   " << refComment << std::endl;
			*m_pOutputStream << "//" << std::endl;
			*m_pOutputStream << "///////////////////////////////////////////////////////////////////////////" << std::endl;
			*m_pOutputStream << " -->" << std::endl;
		}
	}




	//--
	// [rad] Adds a comment
	void Configuration::WriteComment(const std::string& refComment, const std::string& refNamespace, const std::string& refItem)
	{
		if(m_objOptions.m_bGenerateComments)
		{
			*m_pOutputStream << "<!-- " << refComment << ": " << refNamespace << "#" << refItem << " -->" << std::endl;
		}
	}





	//--
	// [rad] Write classes
	void Configuration::WriteBlockClasses()
	{
		WriteCommentBlock("OWL Classes");
		WriteSpace();

		// [rad] we need to filter out unique classes
		std::set<std::string> setClasses;

		for(int i = 0; i < m_vecClassResources.size(); i++) 
		{
			m_vecClassResources[i]->RetrieveClass(setClasses);
		}

		std::set<std::string>::iterator iter_properties;
		for(iter_properties = setClasses.begin(); iter_properties != setClasses.end(); iter_properties++)
		{
            /*
			*m_pOutputStream << "<owl:Class rdf:about=\"&" << (*iter_properties) << "\" />" << std::endl;
			*m_pOutputStream << std::endl << std::endl;
            */
                        
            *m_pOutputStream << "<owl:Class rdf:about=\"&" << (*iter_properties) << "\">" << std::endl;
            *m_pOutputStream << "    <rdfs:label></rdfs:label>" << std::endl;
            *m_pOutputStream << "    <rdfs:comment></rdfs:comment>" << std::endl;
            *m_pOutputStream << "</owl:Class>" << std::endl;
			*m_pOutputStream << std::endl << std::endl;
		}
	}




	//--
	// [rad] Write data properties
	void Configuration::WriteBlockDataProperties()
	{
		WriteCommentBlock("OWL Data Properties");
		WriteSpace();

		// [rad] we need to filter out unique data properties
		std::set<std::string> setDataProperties;

		for(int i = 0; i < m_vecDataPropertyResources.size(); i++) 
		{
			m_vecDataPropertyResources[i]->RetrieveProperty(setDataProperties);
		}		

		std::set<std::string>::iterator iter_properties;
		for(iter_properties = setDataProperties.begin(); iter_properties != setDataProperties.end(); iter_properties++)
		{
            /*
			*m_pOutputStream << "<owl:DatatypeProperty rdf:about=\"&" << (*iter_properties) << "\" />" << std::endl;
			*m_pOutputStream << std::endl << std::endl;
            */
            
            *m_pOutputStream << "<owl:DatatypeProperty rdf:about=\"&" << (*iter_properties) << "\">" << std::endl;
            *m_pOutputStream << "    <rdfs:label></rdfs:label>" << std::endl;
            *m_pOutputStream << "    <rdfs:comment></rdfs:comment>" << std::endl;
            *m_pOutputStream << "</owl:DatatypeProperty>" << std::endl;
			*m_pOutputStream << std::endl << std::endl;
		}
	}





	//--
	// [rad] Write object properties
	void Configuration::WriteBlockObjectProperties()
	{
		WriteCommentBlock("OWL Object Properties");
		WriteSpace();

		// [rad] we need to filter out unique object properties
		std::set<std::string> setObjectProperties;

		for(int i = 0; i < m_vecObjectPropertyResources.size(); i++) 
		{
			m_vecObjectPropertyResources[i]->RetrieveProperty(setObjectProperties);
		}		

		std::set<std::string>::iterator iter_properties;
		for(iter_properties = setObjectProperties.begin(); iter_properties != setObjectProperties.end(); iter_properties++)
		{
            /*
			*m_pOutputStream << "<owl:ObjectProperty rdf:about=\"&" << (*iter_properties) << "\" />" << std::endl;
			*m_pOutputStream << std::endl << std::endl;
            */
            
            *m_pOutputStream << "<owl:ObjectProperty rdf:about=\"&" << (*iter_properties) << "\">" << std::endl;
            *m_pOutputStream << "    <rdfs:label></rdfs:label>" << std::endl;
            *m_pOutputStream << "    <rdfs:comment></rdfs:comment>" << std::endl;
            *m_pOutputStream << "</owl:ObjectProperty>" << std::endl;
			*m_pOutputStream << std::endl << std::endl;
		}
	}





	//--
	// [rad] Write individuals
	void Configuration::WriteBlockIndividuals()
	{
		WriteCommentBlock("OWL Individuals");
		WriteSpace();


		// [rad] all identifiers - for alldifferent statement
		std::vector<std::string> vecAllDifferentIdentifiers;
        
        std::vector<RelationMolecule*> vecRelationsMolecule;
        std::vector<RelationAtom*> vecRelationsAtom;
        std::vector<RelationBond*> vecRelationsBond;
        std::vector<RelationRing*> vecRelationsRing;
		std::vector<RelationDescriptor*> vecRelationsDescriptor;

        RelationMolecule* pRelationMol = NULL;
		RelationAtom* pRelationAtom = NULL;
		RelationBond* pRelationBond = NULL;
		RelationRing* pRelationRing = NULL;
		RelationDescriptor* pRelationDescriptor = NULL;
        
        std::vector<RelationMolecule*>::iterator iter_rel_molecules, iter_rel_molecules_alt;
        std::vector<RelationAtom*>::iterator iter_rel_atoms, iter_rel_atoms_alt;
        std::vector<RelationBond*>::iterator iter_rel_bonds, iter_rel_bonds_alt;
        std::vector<RelationRing*>::iterator iter_rel_rings, iter_rel_rings_alt;
		std::vector<RelationDescriptor*>::iterator iter_rel_descriptors;


		// [rad] DESCRIPTORS
		for(int i = 0; i < m_vecClassResources.size(); i++) 
		{	
			if(!pRelationDescriptor)
			{
                pRelationDescriptor = new RelationDescriptor();
			}

			// [rad] satisfy classes
			if(m_vecClassResources[i]->Satisfy(m_pMolecule, pRelationDescriptor, m_objOptions))
			{
				// [rad] copy data
                pRelationDescriptor->m_pMol = m_pMolecule;

				// [rad] for this class satisfy data properties
				for(int j = 0; j < m_vecDataPropertyResources.size(); j++)
				{
					// [rad] satisfy data properties
					m_vecDataPropertyResources[j]->Satisfy(m_pMolecule, pRelationDescriptor, m_objOptions);			
				}

				// [rad] store this relation
				vecRelationsDescriptor.push_back(pRelationDescriptor);
				
				pRelationMol = NULL;
				pRelationDescriptor = NULL;
			}
		}	
		
		std::cout << "_ Descriptors, classes and datatypes \n";




		// [rad] MOLECULE
		for(int i = 0; i < m_vecClassResources.size(); i++) 
		{	
			if(!pRelationMol)
			{
                pRelationMol = new RelationMolecule();
			}

			// [rad] satisfy classes
			if(m_vecClassResources[i]->Satisfy(m_pMolecule, pRelationMol, m_objOptions))
			{
				// [rad] copy data
                pRelationMol->m_pMol = m_pMolecule;

				// [rad] for this class satisfy data properties
				for(int j = 0; j < m_vecDataPropertyResources.size(); j++)
				{
					// [rad] satisfy data properties
					m_vecDataPropertyResources[j]->Satisfy(m_pMolecule, pRelationMol, m_objOptions);			
				}

				// [rad] store this relation
				vecRelationsMolecule.push_back(pRelationMol);
				
				pRelationMol = NULL;
			}
		}	

		std::cout << "_ Molecule, classes and datatypes \n";
		
		



        

		// [rad] ATOM
		OpenBabel::OBAtom* pAtom;
		std::vector<OpenBabel::OBNodeBase*>::iterator iter_atom;
		for(pAtom = m_pMolecule->BeginAtom(iter_atom); pAtom; pAtom = m_pMolecule->NextAtom(iter_atom))
		{
			// [rad] satisfy classes
			for(int i = 0; i < m_vecClassResources.size(); i++) 
			{
				if(!pRelationAtom)
				{
					pRelationAtom = new RelationAtom();
				}

				// [rad] satisfy classes
				if(m_vecClassResources[i]->Satisfy(pAtom, pRelationAtom, m_objOptions))
				{
					// [rad] copy data
					pRelationAtom->m_pAtom = pAtom;

					// [rad] for this class satisfy data properties
					for(int j = 0; j < m_vecDataPropertyResources.size(); j++)
					{
						// [rad] satisfy data properties
						m_vecDataPropertyResources[j]->Satisfy(pAtom, pRelationAtom, m_objOptions);			
					}

					// [rad] store this relation
					vecRelationsAtom.push_back(pRelationAtom);
					
					pRelationAtom = NULL;
				}
			}
		}

		std::cout << "_ Atoms, classes and datatypes \n";



	
		// [rad] BOND
		OpenBabel::OBBond* pBond;
		std::vector<OpenBabel::OBEdgeBase*>::iterator iter_bonds;
		for(pBond = m_pMolecule->BeginBond(iter_bonds); pBond; pBond = m_pMolecule->NextBond(iter_bonds))
		{
			// [rad] satisfy classes
			for(int i = 0; i < m_vecClassResources.size(); i++) 
			{
				if(!pRelationBond)
				{
					pRelationBond = new RelationBond();
				}

				// [rad] satisfy classes
				if(m_vecClassResources[i]->Satisfy(pBond, pRelationBond, m_objOptions))
				{
					// [rad] copy data
					pRelationBond->m_pBond = pBond;

					// [rad] for this class satisfy data properties
					for(int j = 0; j < m_vecDataPropertyResources.size(); j++)
					{
						// [rad] satisfy data properties
						m_vecDataPropertyResources[j]->Satisfy(pBond, pRelationBond, m_objOptions);			
					}

					// [rad] store this relation
					vecRelationsBond.push_back(pRelationBond);
					
					pRelationBond = NULL;
				}
			}
		}

		std::cout << "_ Bonds, classes and datatypes \n";




		


		// [rad] RING
		std::vector<OpenBabel::OBRing*>& vRings = m_pMolecule->GetSSSR();
		std::vector<OpenBabel::OBRing*>::iterator iter_rings;
		for(iter_rings = vRings.begin(); iter_rings != vRings.end(); iter_rings++)
		{
			// [rad] satisfy classes
			for(int i = 0; i < m_vecClassResources.size(); i++) 
			{
				if(!pRelationRing)
				{
					pRelationRing = new RelationRing();
				}

				// [rad] satisfy classes
				if(m_vecClassResources[i]->Satisfy((*iter_rings), pRelationRing, m_objOptions))
				{
					// [rad] copy data
					pRelationRing->m_pRing = (*iter_rings);

					// [rad] for this class satisfy data properties
					for(int j = 0; j < m_vecDataPropertyResources.size(); j++)
					{
						// [rad] satisfy data properties
						m_vecDataPropertyResources[j]->Satisfy((*iter_rings), pRelationRing, m_objOptions);			
					}

					// [rad] store this relation
                    vecRelationsRing.push_back(pRelationRing);
					
					pRelationRing = NULL;
				}
			}
		}

		std::cout << "_ Rings, classes and datatypes \n";



		// [rad] MOLECULE 
		for(iter_rel_molecules = vecRelationsMolecule.begin(); iter_rel_molecules != vecRelationsMolecule.end(); iter_rel_molecules++)
		{
			// [rad] <-> DESCRIPTOR
			// [rad] It is a somewhat special case, since we only need to check descriptors vs molecules
			for(iter_rel_descriptors = vecRelationsDescriptor.begin(); iter_rel_descriptors != vecRelationsDescriptor.end(); iter_rel_descriptors++)
			{
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy(*iter_rel_molecules, *iter_rel_descriptors, m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy(*iter_rel_molecules, *iter_rel_descriptors, m_objOptions, false);
				}
			}
			
			// [rad] <-> MOLECULE 
			for(iter_rel_molecules_alt = vecRelationsMolecule.begin(); iter_rel_molecules_alt != vecRelationsMolecule.end(); iter_rel_molecules_alt++)
			{
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy(*iter_rel_molecules, *iter_rel_molecules_alt, m_objOptions);
					//m_vecObjectPropertyResources[i]->Satisfy(*iter_rel_molecules, *iter_rel_molecules_alt, m_objOptions, false);
				}
			}

			// [rad] <-> ATOM
			for(iter_rel_atoms = vecRelationsAtom.begin(); iter_rel_atoms != vecRelationsAtom.end(); iter_rel_atoms++)
			{
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy(*iter_rel_molecules, *iter_rel_atoms, m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy(*iter_rel_molecules, *iter_rel_atoms, m_objOptions, false);
				}
			}

			// [rad] <-> BOND
			for(iter_rel_bonds = vecRelationsBond.begin(); iter_rel_bonds != vecRelationsBond.end(); iter_rel_bonds++)
			{
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_molecules), (*iter_rel_bonds), m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_molecules), (*iter_rel_bonds), m_objOptions, false);
				}
			}

			// [rad] <-> RING
			for(iter_rel_rings = vecRelationsRing.begin(); iter_rel_rings != vecRelationsRing.end(); iter_rel_rings++)
			{
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_molecules), (*iter_rel_rings), m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_molecules), (*iter_rel_rings), m_objOptions, false);
				}
			}			
		}
		
		std::cout << "_ Molecule, relations \n";


		// [rad] ATOM
		for(iter_rel_atoms = vecRelationsAtom.begin(); iter_rel_atoms != vecRelationsAtom.end(); iter_rel_atoms++)
		{
			// [rad] <-> ATOM
			for(iter_rel_atoms_alt = vecRelationsAtom.begin(); iter_rel_atoms_alt != vecRelationsAtom.end(); iter_rel_atoms_alt++)
			{
                
				// [rad] only check those that have the same atom
				//if((*iter_rel_atoms_alt)->GetAtom() != (*iter_rel_atoms)->GetAtom()) continue;
                
                // [rad] only check those that have the same atom or have a shared bond
                if(((*iter_rel_atoms_alt)->GetAtom() != (*iter_rel_atoms)->GetAtom()) &&
                    !((*iter_rel_atoms)->GetAtom()->GetBond((*iter_rel_atoms_alt)->GetAtom()))) continue;
				
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_atoms), (*iter_rel_atoms_alt), m_objOptions);
					//m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_atoms), (*iter_rel_atoms_alt), false);
				}
                
			}
			
						
			
		
			// [rad] <-> BOND
			for(iter_rel_bonds = vecRelationsBond.begin(); iter_rel_bonds != vecRelationsBond.end(); iter_rel_bonds++)
			{
				// [rad] only check those that have links
				if((*iter_rel_atoms)->GetAtom() != (*iter_rel_bonds)->GetBond()->GetBeginAtom() &&
					(*iter_rel_atoms)->GetAtom() != (*iter_rel_bonds)->GetBond()->GetEndAtom()) continue;
				
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_atoms), (*iter_rel_bonds), m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_atoms), (*iter_rel_bonds), m_objOptions, false);
				}
			}
			

			for(iter_rel_rings = vecRelationsRing.begin(); iter_rel_rings != vecRelationsRing.end(); iter_rel_rings++)
			{
				// [rad] only check those that have links
				if(!(*iter_rel_rings)->GetRing()->IsMember((*iter_rel_atoms)->GetAtom())) continue;
				
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_atoms), (*iter_rel_rings), m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_atoms), (*iter_rel_rings), m_objOptions, false);
				}
			}		
		}
		
		std::cout << "_ Atom, relations \n";



		// [rad] BOND
		for(iter_rel_bonds = vecRelationsBond.begin(); iter_rel_bonds != vecRelationsBond.end(); iter_rel_bonds++)
		{
			for(iter_rel_bonds_alt = vecRelationsBond.begin(); iter_rel_bonds_alt != vecRelationsBond.end(); iter_rel_bonds_alt++)
			{
				// [rad] only check those that have links
				if((*iter_rel_bonds)->GetBond() != (*iter_rel_bonds_alt)->GetBond()) continue;
				
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_bonds), (*iter_rel_bonds_alt), m_objOptions);
					//m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_bonds), (*iter_rel_bonds_alt), false);
				}
			}


			// [rad] <-> RING
			for(iter_rel_rings = vecRelationsRing.begin(); iter_rel_rings != vecRelationsRing.end(); iter_rel_rings++)
			{
				// [rad] only check those that have links
				if(!(*iter_rel_rings)->GetRing()->IsMember((*iter_rel_bonds)->GetBond())) continue;
				
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_bonds), (*iter_rel_rings), m_objOptions);
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_bonds), (*iter_rel_rings), m_objOptions, false);
				}
			}
		}
		
		std::cout << "_ Bond, relations \n";


		// [rad] RING
		for(iter_rel_rings = vecRelationsRing.begin(); iter_rel_rings != vecRelationsRing.end(); iter_rel_rings++)
		{
			// [rad] only check those that have links
			if((*iter_rel_rings)->GetRing() != (*iter_rel_rings)->GetRing()) continue;

			// [rad] <-> RING
			for(iter_rel_rings_alt = vecRelationsRing.begin(); iter_rel_rings_alt != vecRelationsRing.end(); iter_rel_rings_alt++)
			{
				for(int i = 0; i < m_vecObjectPropertyResources.size(); i++)
				{
					m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_rings), (*iter_rel_rings_alt), m_objOptions);
					//m_vecObjectPropertyResources[i]->Satisfy((*iter_rel_rings), (*iter_rel_rings_alt), false);
				}
			}			
		}

		std::cout << "_ Ring, relations \n";




        // [rad] Serializing satisfied members
        for(iter_rel_descriptors = vecRelationsDescriptor.begin(); iter_rel_descriptors != vecRelationsDescriptor.end(); iter_rel_descriptors++)
		{
			WriteIndividualDescriptor((*iter_rel_descriptors));
			WriteSpace();
		}
		
		
		for(iter_rel_molecules = vecRelationsMolecule.begin(); iter_rel_molecules != vecRelationsMolecule.end(); iter_rel_molecules++)
		{
			WriteIndividualMolecule((*iter_rel_molecules));
			WriteSpace();
		}

		for(iter_rel_atoms = vecRelationsAtom.begin(); iter_rel_atoms != vecRelationsAtom.end(); iter_rel_atoms++)
		{
			WriteIndividualAtom((*iter_rel_atoms));
			WriteSpace();
		}

		for(iter_rel_bonds = vecRelationsBond.begin(); iter_rel_bonds != vecRelationsBond.end(); iter_rel_bonds++)
		{
			WriteIndividualBond((*iter_rel_bonds));
			WriteSpace();
		}

		for(iter_rel_rings = vecRelationsRing.begin(); iter_rel_rings != vecRelationsRing.end(); iter_rel_rings++)
		{
			WriteIndividualRing((*iter_rel_rings));
			WriteSpace();
		}



		
		// [rad] check flag - if we need to add all-different section
		if(m_objOptions.m_bGenerateAllDifferent)
		{
			// [rad] do we have anything to serialize?
			if(!vecRelationsMolecule.size() && !vecRelationsAtom.size() 
					&& !vecRelationsBond.size() && !vecRelationsRing.size())
			{
				// [rad] nothing to serialize
				return;
			}

			std::string sPath, sIdent;

			WriteCommentBlock("OWL Axioms");
			WriteSpace();

			*m_pOutputStream << "<owl:AllDifferent>" << std::endl;
			*m_pOutputStream << "    <owl:distinctMembers rdf:parseType=\"Collection\">" << std::endl;

			for(iter_rel_molecules = vecRelationsMolecule.begin(); iter_rel_molecules != vecRelationsMolecule.end(); iter_rel_molecules++)
			{
				(*iter_rel_molecules)->GetPathAndIdentMolecule(sPath, sIdent);
				*m_pOutputStream << "        <owl:Thing rdf:about=\"#" << sIdent << "\" />" << std::endl;
			}

			for(iter_rel_atoms = vecRelationsAtom.begin(); iter_rel_atoms != vecRelationsAtom.end(); iter_rel_atoms++)
			{
				(*iter_rel_atoms)->GetPathAndIdentAtom(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "        <owl:Thing rdf:about=\"#" << sIdent << "\" />" << std::endl;
			}

			for(iter_rel_bonds = vecRelationsBond.begin(); iter_rel_bonds != vecRelationsBond.end(); iter_rel_bonds++)
			{
				(*iter_rel_bonds)->GetPathAndIdentBond(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "        <owl:Thing rdf:about=\"#" << sIdent << "\" />" << std::endl;
			}

			for(iter_rel_rings = vecRelationsRing.begin(); iter_rel_rings != vecRelationsRing.end(); iter_rel_rings++)
			{
				(*iter_rel_rings)->GetPathAndIdentRing(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "        <owl:Thing rdf:about=\"#" << sIdent << "\" />" << std::endl;
			}

			*m_pOutputStream << "    </owl:distinctMembers>" << std::endl;
			*m_pOutputStream << "</owl:AllDifferent>" << std::endl;
		}
        
		
		
		// [rad] clean up..
		iter_rel_molecules = vecRelationsMolecule.begin();
        while(iter_rel_molecules != vecRelationsMolecule.end())
        {
            delete(*iter_rel_molecules);
            iter_rel_molecules = vecRelationsMolecule.erase(iter_rel_molecules);
        }
		
		iter_rel_atoms = vecRelationsAtom.begin();
        while(iter_rel_atoms != vecRelationsAtom.end())
        {
            delete(*iter_rel_atoms);
            iter_rel_atoms = vecRelationsAtom.erase(iter_rel_atoms);
        }
		
		
		iter_rel_descriptors = vecRelationsDescriptor.begin();
        while(iter_rel_descriptors != vecRelationsDescriptor.end())
        {
            delete(*iter_rel_descriptors);
            iter_rel_descriptors = vecRelationsDescriptor.erase(iter_rel_descriptors);
        }
		
		
		iter_rel_bonds = vecRelationsBond.begin();
        while(iter_rel_bonds != vecRelationsBond.end())
        {
            delete(*iter_rel_bonds);
            iter_rel_bonds = vecRelationsBond.erase(iter_rel_bonds);
        }
		
		iter_rel_rings = vecRelationsRing.begin();
        while(iter_rel_rings != vecRelationsRing.end())
        {
            delete(*iter_rel_rings);
            iter_rel_rings = vecRelationsRing.erase(iter_rel_rings);
        }
	}




	//--
	void Configuration::WriteIndividualDescriptor(RelationDescriptor* pRelDescriptor)
	{
		std::string sValue, sType;
		std::string sPath, sIdent;
		
		if(pRelDescriptor->m_vecSatisfiedClasses.size())
		{
			pRelDescriptor->GetPathAndIdentDescriptor(sPath, sIdent);
			*m_pOutputStream << "<" << sPath << " rdf:about=\"#" << sIdent;
		
			// [rad] if no data/object properties or additional classes, close
			if(pRelDescriptor->IsTrivial())
			{
				*m_pOutputStream << "\" />" << std::endl;
				return;
			}
			
			*m_pOutputStream << "\">" << std::endl;
			
			if(pRelDescriptor->HasExtraClasses())
			{
				*m_pOutputStream << std::endl;
			}
			
			// [rad] output additional class defs
			for(int i = 1; i < pRelDescriptor->m_vecSatisfiedClasses.size(); i++)
			{
				pRelDescriptor->GetClassPath(i, sIdent);
				*m_pOutputStream << "         <rdf:type rdf:resource=\"&" << sIdent << "\" />" << std::endl;
			}
			
			if(pRelDescriptor->HasDataProperties())
			{
				*m_pOutputStream << std::endl;
			}

			
			// [rad] output data properties
			for(int i = 0; i < pRelDescriptor->m_vecSatisfiedDataProperties.size(); i++)
			{
				pRelDescriptor->GetDataProperty(i, sPath, sType, sValue);
				*m_pOutputStream << "         <" << sPath << " rdf:datatype=\"&xsd;" << sType << "\">" 
						<< sValue << "</" << sPath << ">"<< std::endl;
			}

			if(pRelDescriptor->HasObjectProperties())
			{
				*m_pOutputStream << std::endl;
			}


			// [rad] only molecule object properties are possible with descriptors
			for(int i = 0; i < pRelDescriptor->m_vecSatisfiedObjectPropertiesMolecule.size(); i++)
			{
				pRelDescriptor->GetObjectPropertyPathAndIdentMolecule(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}



			*m_pOutputStream << std::endl;

			// [rad] close tag
			pRelDescriptor->GetPathAndIdentDescriptor(sPath, sIdent);
			*m_pOutputStream << "</" << sPath << ">" << std::endl;

		
		}
	
	}
	
	
	

	//--
    void Configuration::WriteIndividualMolecule(RelationMolecule* pRelMolecule)
	{        
		std::string sValue, sType;
		std::string sPath, sIdent;

		if(pRelMolecule->m_vecSatisfiedClasses.size())
		{
			pRelMolecule->GetPathAndIdentMolecule(sPath, sIdent);
			*m_pOutputStream << "<" << sPath << " rdf:about=\"#" << sIdent;

			// [rad] if no data/object properties or additional classes, close
			if(pRelMolecule->IsTrivial())
			{
				*m_pOutputStream << "\" />" << std::endl;
				return;
			}

			*m_pOutputStream << "\">" << std::endl;
			
			if(pRelMolecule->HasExtraClasses())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output additional class defs
			for(int i = 1; i < pRelMolecule->m_vecSatisfiedClasses.size(); i++)
			{
				pRelMolecule->GetClassPath(i, sIdent);
				*m_pOutputStream << "         <rdf:type rdf:resource=\"&" << sIdent << "\" />" << std::endl;
			}



			if(pRelMolecule->HasDataProperties())
			{
				*m_pOutputStream << std::endl;
			}

			
			// [rad] output data properties
			for(int i = 0; i < pRelMolecule->m_vecSatisfiedDataProperties.size(); i++)
			{
				pRelMolecule->GetDataProperty(i, sPath, sType, sValue);
				*m_pOutputStream << "         <" << sPath << " rdf:datatype=\"&xsd;" << sType << "\">" 
						<< sValue << "</" << sPath << ">"<< std::endl;
			}

			if(pRelMolecule->HasObjectProperties())
			{
				*m_pOutputStream << std::endl;
			}
			
			// [rad] output molecule properties
			for(int i = 0; i < pRelMolecule->m_vecSatisfiedObjectPropertiesMolecule.size(); i++)
			{
				pRelMolecule->GetObjectPropertyPathAndIdentMolecule(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}


			
			// [rad] output atom properties
			for(int i = 0; i < pRelMolecule->m_vecSatisfiedObjectPropertiesDescriptor.size(); i++)
			{
				pRelMolecule->GetObjectPropertyPathAndIdentDescriptor(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}
			
			
			
			
			// [rad] output atom properties
			for(int i = 0; i < pRelMolecule->m_vecSatisfiedObjectPropertiesAtom.size(); i++)
			{
				pRelMolecule->GetObjectPropertyPathAndIdentAtom(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}



			// [rad] output bond properties
			for(int i = 0; i < pRelMolecule->m_vecSatisfiedObjectPropertiesBond.size(); i++)
			{
				pRelMolecule->GetObjectPropertyPathAndIdentBond(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}



			// [rad] output ring properties
			for(int i = 0; i < pRelMolecule->m_vecSatisfiedObjectPropertiesRing.size(); i++)
			{
				pRelMolecule->GetObjectPropertyPathAndIdentRing(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}


			*m_pOutputStream << std::endl;

			// [rad] do we need to generate inchi mapping?
			if(m_objOptions.m_bGenerateInchiMapping)
			{
				std::vector<OpenBabel::OBGenericData*>::iterator iter_data;
				std::string sQualityName, sQualityValue;
				OpenBabel::OBPairData* pAuxData;

				bool bFound = false;
				for(iter_data = pRelMolecule->m_pMol->BeginData(); iter_data != pRelMolecule->m_pMol->EndData(); iter_data++)
				{
					// [rad] Take care of PairData only for now
					if(1 == (*iter_data)->GetDataType())
					{
						pAuxData = (OpenBabel::OBPairData*)(*iter_data);
						sQualityName = pAuxData->GetAttribute();
						sQualityValue = pAuxData->GetValue();

						if(!sQualityName.compare("PUBCHEM_NIST_INCHI")) 
						{
							bFound = true;
							break;
						}
					}
				}

				// [rad] we have inchi information
				if(bFound)
				{
					*m_pOutputStream << "         <owl:sameAs rdf:resource=\"inchi:" << sQualityValue << "\" />" << std::endl;
					*m_pOutputStream << std::endl;
				}
			}

			// [rad] close tag
			pRelMolecule->GetPathAndIdentMolecule(sPath, sIdent);
			*m_pOutputStream << "</" << sPath << ">" << std::endl;
		}
	}




	//--
	void Configuration::WriteIndividualAtom(RelationAtom* pRelAtom)
	{
    	std::string sPath, sIdent;
		std::string sValue, sType;

		if(pRelAtom->m_vecSatisfiedClasses.size())
		{
			pRelAtom->GetPathAndIdentAtom(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
			*m_pOutputStream << "<" << sPath << " rdf:about=\"#" << sIdent;

			// [rad] if no data/object properties or additional classes, close
			if(pRelAtom->IsTrivial())
			{
				*m_pOutputStream << "\" />" << std::endl;
				return;
			}

			*m_pOutputStream << "\">" << std::endl;
			
			if(pRelAtom->HasExtraClasses())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output additional class defs
			for(int i = 1; i < pRelAtom->m_vecSatisfiedClasses.size(); i++)
			{
				pRelAtom->GetClassPath(i, sIdent);
				*m_pOutputStream << "         <rdf:type rdf:resource=\"&" << sIdent << "\" />" << std::endl;
			}

			if(pRelAtom->HasDataProperties())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output data properties
			for(int i = 0; i < pRelAtom->m_vecSatisfiedDataProperties.size(); i++)
			{
				pRelAtom->GetDataProperty(i, sPath, sType, sValue);
				*m_pOutputStream << "         <" << sPath << " rdf:datatype=\"&xsd;" << sType << "\">" 
						<< sValue << "</" << sPath << ">"<< std::endl;
			}
			
			
			if(pRelAtom->HasObjectProperties())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output molecule properties
			for(int i = 0; i < pRelAtom->m_vecSatisfiedObjectPropertiesMolecule.size(); i++)
			{
				pRelAtom->GetObjectPropertyPathAndIdentMolecule(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refAtom->m_vecSatisfiedObjectPropertiesAtom.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output atom properties
			for(int i = 0; i < pRelAtom->m_vecSatisfiedObjectPropertiesAtom.size(); i++)
			{
				pRelAtom->GetObjectPropertyPathAndIdentAtom(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refAtom->m_vecSatisfiedObjectPropertiesBond.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output bond properties
			for(int i = 0; i < pRelAtom->m_vecSatisfiedObjectPropertiesBond.size(); i++)
			{
				pRelAtom->GetObjectPropertyPathAndIdentBond(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refAtom->m_vecSatisfiedObjectPropertiesRing.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output ring properties
			for(int i = 0; i < pRelAtom->m_vecSatisfiedObjectPropertiesRing.size(); i++)
			{
				pRelAtom->GetObjectPropertyPathAndIdentRing(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}


			*m_pOutputStream << std::endl;

			// [rad] close tag
			pRelAtom->GetPathAndIdentAtom(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
			*m_pOutputStream << "</" << sPath << ">" << std::endl;
		}
	}




	//--
	void Configuration::WriteIndividualBond(RelationBond* pRelBond)
	{
        
		std::string sPath, sIdent;
		std::string sValue, sType;

		if(pRelBond->m_vecSatisfiedClasses.size())
		{
			pRelBond->GetPathAndIdentBond(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
			*m_pOutputStream << "<" << sPath << " rdf:about=\"#" << sIdent;

			// [rad] if no data/object properties or additional classes, close
			if(pRelBond->IsTrivial())
			{
				*m_pOutputStream << "\" />" << std::endl;
				return;
			}

			*m_pOutputStream << "\">" << std::endl;
			
			if(pRelBond->HasExtraClasses())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output additional class defs
			for(int i = 1; i < pRelBond->m_vecSatisfiedClasses.size(); i++)
			{
				pRelBond->GetClassPath(i, sIdent);
				*m_pOutputStream << "         <rdf:type rdf:resource=\"&" << sIdent << "\" />" << std::endl;
			}

			if(pRelBond->HasDataProperties())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output data properties
			for(int i = 0; i < pRelBond->m_vecSatisfiedDataProperties.size(); i++)
			{
				pRelBond->GetDataProperty(i, sPath, sType, sValue);
				*m_pOutputStream << "         <" << sPath << " rdf:datatype=\"&xsd;" << sType << "\">" 
						<< sValue << "</" << sPath << ">"<< std::endl;
			}
			
			
			if(pRelBond->HasObjectProperties())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output molecule properties
			for(int i = 0; i < pRelBond->m_vecSatisfiedObjectPropertiesMolecule.size(); i++)
			{
				pRelBond->GetObjectPropertyPathAndIdentMolecule(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refBond->m_vecSatisfiedObjectPropertiesAtom.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output atom properties
			for(int i = 0; i < pRelBond->m_vecSatisfiedObjectPropertiesAtom.size(); i++)
			{
				pRelBond->GetObjectPropertyPathAndIdentAtom(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refBond->m_vecSatisfiedObjectPropertiesBond.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output bond properties
			for(int i = 0; i < pRelBond->m_vecSatisfiedObjectPropertiesBond.size(); i++)
			{
				pRelBond->GetObjectPropertyPathAndIdentBond(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refBond->m_vecSatisfiedObjectPropertiesRing.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output ring properties
			for(int i = 0; i < pRelBond->m_vecSatisfiedObjectPropertiesRing.size(); i++)
			{
				pRelBond->GetObjectPropertyPathAndIdentRing(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}


			*m_pOutputStream << std::endl;

			// [rad] close tag
			pRelBond->GetPathAndIdentBond(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
			*m_pOutputStream << "</" << sPath << ">" << std::endl;
		}
	}




	//--
	void Configuration::WriteIndividualRing(RelationRing* pRelRing)
	{
        
		std::string sPath, sIdent;
		std::string sValue, sType;

		if(pRelRing->m_vecSatisfiedClasses.size())
		{
			pRelRing->GetPathAndIdentRing(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
			*m_pOutputStream << "<" << sPath << " rdf:about=\"#" << sIdent;

			// [rad] if no data/object properties or additional classes, close
			if(pRelRing->IsTrivial())
			{
				*m_pOutputStream << "\" />" << std::endl;
				return;
			}

			*m_pOutputStream << "\">" << std::endl;
			
			if(pRelRing->HasExtraClasses())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output additional class defs
			for(int i = 1; i < pRelRing->m_vecSatisfiedClasses.size(); i++)
			{
				pRelRing->GetClassPath(i, sIdent);
				*m_pOutputStream << "         <rdf:type rdf:resource=\"&" << sIdent << "\" />" << std::endl;
			}

			if(pRelRing->HasDataProperties())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output data properties
			for(int i = 0; i < pRelRing->m_vecSatisfiedDataProperties.size(); i++)
			{
				pRelRing->GetDataProperty(i, sPath, sType, sValue);
				*m_pOutputStream << "         <" << sPath << " rdf:datatype=\"&xsd;" << sType << "\">" 
						<< sValue << "</" << sPath << ">"<< std::endl;
			}
			
			
			if(pRelRing->HasObjectProperties())
			{
				*m_pOutputStream << std::endl;
			}

			// [rad] output molecule properties
			for(int i = 0; i < pRelRing->m_vecSatisfiedObjectPropertiesMolecule.size(); i++)
			{
				pRelRing->GetObjectPropertyPathAndIdentMolecule(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refRing->m_vecSatisfiedObjectPropertiesAtom.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output atom properties
			for(int i = 0; i < pRelRing->m_vecSatisfiedObjectPropertiesAtom.size(); i++)
			{
				pRelRing->GetObjectPropertyPathAndIdentAtom(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refRing->m_vecSatisfiedObjectPropertiesBond.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output bond properties
			for(int i = 0; i < pRelRing->m_vecSatisfiedObjectPropertiesBond.size(); i++)
			{
				pRelRing->GetObjectPropertyPathAndIdentBond(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}

			//if(refRing->m_vecSatisfiedObjectPropertiesRing.size())
			//{
			//	*m_pOutputStream << std::endl;
			//}

			// [rad] output ring properties
			for(int i = 0; i < pRelRing->m_vecSatisfiedObjectPropertiesRing.size(); i++)
			{
				pRelRing->GetObjectPropertyPathAndIdentRing(i, std::string(m_pMolecule->GetTitle()), sPath, sIdent);
				*m_pOutputStream << "         <" << sPath << " rdf:resource=\"#" << sIdent << "\" />" << std::endl;
			}


			*m_pOutputStream << std::endl;

			// [rad] close tag
			pRelRing->GetPathAndIdentRing(std::string(m_pMolecule->GetTitle()), sPath, sIdent);
			*m_pOutputStream << "</" << sPath << ">" << std::endl;
		}
	}





}