# MinibexTranslator

Some piece of code to translate ibex library (http://www.ibex-lib.org) domain specific langage, namely Minibex, to some other languages.

Current compatible release of ibex: 2.8.7

Current supported languages are:

- AMPL
- GAMS


## Usage

Currently the makefile will generate several binaries:

- ExprAMPL: basic tests the class that writes AMPL expressions
- ExprGAMS: basic tests the class that writes GAMS expressions
- Minibex2AMPL: it expects a minibex filename and generate a mod file
- Minibex2GAMS: it expects a minibex filename and generate a gms file
