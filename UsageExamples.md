# Introduction #

You should be able to use the OWL/RDF plugin just like any other Open Babel plugin (the only limitation is that it's a write-only plugin at the moment).


# Usage #

The general usage is:


<ob\_executable>

 -i

<input\_format>

 

<input\_file>

 -oowl 

<owl\_file>

 -q

<config\_file>



-q argument is optional, if it's skipped, configuration will be read from `OwlPluginConfiguration.xml` file, which should be located in the same directory as the Open Babel executable.

# Examples #

Here we assume that 'babel' is the name of our Open Babel executable.


  1. Read `CID_1549520.sdf`, write to `CID_1549520.owl`, use `owl_conf2.xml` as config:
> _babel -isdf `CID_1549520.sdf` -oowl `CID_1549520.owl` -q”`owl_conf2.xml`”_

  1. Read `CID_1549520.sdf`, write to `CID_1549520.owl`, use `OwlPluginConfiguration.xml` as config:
> _babel -isdf `CID_1549520.sdf` -oowl `CID_1549520.owl`_

  1. Read `CID_1549520.sdf` and output resulting owl file to console.
> _babel -isdf `CID_1549520.sdf` -oowl_