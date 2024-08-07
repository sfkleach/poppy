#include <iostream>
#include <sstream>
#include <iomanip>
#include <cstring> 
#include <cstdarg>
#include <cstdlib>

#include "mishap.hpp"

namespace poppy {

using namespace std;

#define PREFIX "    "
#define ORIGIN ( PREFIX "Cause" )
#define SYS_MSG_PREFIX "### "

Mishap & Mishap::cause( Mishap & problem ) {
	this->culprit( ORIGIN, problem.getMessage() );
	for ( 	
		vector< pair< string, string > >::iterator it = problem.culprits.begin();
		it != problem.culprits.end();
		++it
	) {
		std::string key( PREFIX );
		key.append( it->first );
		this->culprit( key, it->second );
	}
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const std::string arg ) {
	this->culprits.push_back( pair< const string, const string >( reason, arg ) );
	return *this;
}

Mishap & Mishap::culprit( const char * reason, const std::string arg ) {
	this->culprits.push_back( pair< const string, const string >( reason, arg ) );
	return *this;
}

Mishap & Mishap::culprit( const char * reason, const char * arg ) {
	this->culprits.push_back( pair< const string, const string >( reason, arg ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const signed long N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const uint64_t N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const int64_t N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const unsigned long N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const signed int N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const unsigned int N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string reason, const double N ) {
	std::ostringstream s;
	s << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culpritChar( const std::string reason, const char N ) {
	std::ostringstream s;
	s << "character " << N;
	const std::string result( s.str() );
	this->culprits.push_back( pair< const string, const string >( reason, result ) );
	return *this;
}

Mishap & Mishap::culprit( const std::string arg ) {
	this->culprits.push_back( std::pair< const string, const string >( "Argument", arg ) );
	return *this;
}

std::string Mishap::severity() const {
	switch ( this->mishap_severity ) {
		case EXECUTION_TIME_SEVERITY:
			return "Mishap";
		case SYSTEM_ERROR_SEVERITY:
			return "Panic";
		case COMPILE_TIME_SEVERITY:
			return "Compilation Error";
		default:
			return "(Unknown)";
	}
}


void Mishap::report() {

	size_t width = this->severity().size();
	for ( 	
		vector< pair< string, string > >::iterator it = this->culprits.begin();
		it != this->culprits.end();
		++it
	) {
		size_t n = it->first.size();
		if ( n > width ) width = n;
	}
	

	cerr << endl;
	cerr << SYS_MSG_PREFIX << left << setw( width ) << this->severity() << " : " << this->message << endl;
	for ( 	
		vector< pair< string, string > >::iterator it = this->culprits.begin();
		it != this->culprits.end();
		++it
	) {
		cerr << SYS_MSG_PREFIX << left << setw( width ) << it->first << " : " << it->second << endl;
	}
	cerr << endl;
}

std::string Mishap::getMessage() {
	return this->message;
}

std::pair< std::string, std::string > & Mishap::getCulprit( int n ) {
	return this->culprits[ n ];
}

int Mishap::getCount() {
	return this->culprits.size();
}

} // namespace poppy