# dtack-base-cpp

Base utility function library in C++.

## FAQ
________________________________________________________________________
##### Why was this library written?

To fulfill the need for basic common utlities in C++. 

________________________________________________________________________
##### What functional components are implemented by this library?

component | purpose
---   | ---
incidents | Rolling and cumulative runtime metrics.
environment | runtime environment provided to all instances
base | base class for all classes
diwec_logging | logging like python (Debug Info Warning Error Critical)
configuration | configuration as read from command line or file (deprecated in favor of limn)
limn | roughly equivalent to Python dictionary
dispatcher | interface for dispatchers which take commands and call functions
string | string parsing and tokenizing
packet | structured parsing, access and composing to json
testing | support for programs which perform testing
cli | support for programs which are run at the command line
bytes_reader | implements reading of a byte stream by file or socket
version | Version of the libary

___________________________________________________________________________________
##### Why not have separate repos and libraries for each of the components above?

Probably a good idea.  Cost is extra work and there is not likely to be an explosion of users for this software.

_____________________________________________________________
##### How is the code organized?

The library is in `src`.  Each component has its own subdirectory there.  There is a single Makefile for all of src.  Output compiles to `lib`.

Each test lives in its own subdirectory of `tests`.  Each test subdirectory has its own Makefile. Output compiles to `bin`.

_____________________________________________________________
##### How do I compile the library and run the tests?

`make all`

`make gitlab_ci_tests`

________________________________________________________________________
##### What is the binary filename?

`lib/libdtack.base.so`

________________________________________________________________________
##### What on the TODO list?

* Better documentation.
* Implement Configuration in terms of Limn.

________________________________________________________________________
##### What changes have been made on this body of software?

* 2021-01-04 Fix memory leak in Packet class.









