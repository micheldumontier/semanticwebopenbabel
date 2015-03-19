# Current Limitations #

Currently the OWL generator plugin is a write-only plugin, meaning it can output RDF/OWL files, however it cannot read these back into the Open Babel data model. Although, having the initial configuration file (which was used during the generation stage) it should not be difficult to do the reverse procedure. An interesting option would be to embed the original configuration file as a comment inside the generated OWL ontology, so it could later be used to read the data back into the Open Babel data model.

In this project we haven’t addressed several things which might eventually prove to be useful: for example, ability to tell plugin to “close the world” and ability to generate Qualified Cardinality Restrictions (QCRs).