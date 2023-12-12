from contextlib import contextmanager
from typing import List
from abc import ABC, abstractmethod
import json
import sys

### Nodes represent the structure of the if-then-else decision tree.

class Writer:

    def __init__( self, file, class_name, map_info, extra_field_names ):
        self._file = file
        self._indent = 0
        self._map_info = map_info
        self._class_name = class_name
        self._extra_field_names = extra_field_names

    def indent( self ):
        self._indent += 1

    def dedent( self ):
        self._indent -= 1

    def writeLine( self, *parts ):
        self.write( *parts )
        self.newline()

    def write( self, *parts ):
        for i in range( 0, self._indent ):
            print( '    ', end = "", file = self._file )
        for s in parts:
            print( s, end = "", file = self._file )

    def __call__( self, *parts ):
        self.writeLine( *parts )

    def newline( self ):
        print( file = self._file )

    def code( self, x ):
        return f"""{self._class_name}::{self._map_info[x]['code']}"""
 
    def fields( self, x ):
        for field_name in self._extra_field_names:
            self( f"""{field_name} = {self._map_info[x][field_name]};""" )

@contextmanager
def indent( writer: Writer ):
    writer.indent()
    yield writer
    writer.dedent()

class Node( ABC ):

    def indent( self, indent: int, *, file,  ):
        for i in range( 0, indent ):
            print( '    ', end = "", file = file )
    
    @abstractmethod
    def generateCode( self, writer: Writer ):
        ...

"""This represents a binary chop at a particular character position."""
class IfThenElse( Node ):
        
    def __init__( self, condition: (int, str), min_len: int, then_part: Node, else_part: Node ):
        self._condition = condition
        self_min_len = min_len
        self._then_part = then_part
        self._else_part = else_part

    def generateCode( self, writer: Writer ):
        ( i, low_char ) = self._condition
        low_char_in_cpp = low_char if low_char != '\0' else '\\0'
        if i == 0:
            writer( f"""if ( s[{i}] <= '{low_char_in_cpp}' ) {{""" )
        else:
            writer( f"""if ( len <= {i} || s[{i}] <= '{low_char_in_cpp}' ) {{""" )
        with indent(writer):
            self._then_part.generateCode( writer )
        writer( f"""}} else {{""" )
        with indent(writer):
            self._else_part.generateCode( writer )
        writer( f"""}}""" )

"""This represents a switch on the length of the string."""
class SwitchOnLength( Node ):
    
    def __init__( self, cases: dict ):
        self._cases = cases

    def generateCode( self, writer: Writer ):
        writer( f"""switch ( len ) {{""" )
        for k, v in self._cases.items():
            with indent(writer):
                writer( f"""case {k}: {{""" )
                with indent(writer):
                    v.generateCode( writer )
                writer( f"""}} break;""" )
        writer( f"""default: return false;""" )
        writer( f"""}}""" )

"""
If this is a single word, we should test for this. Otherwise we failed to 
find a good decision, just do a linear search.
"""
class OneOf( Node ):

    def __init__( self, choices: List[ str ] ):
        self._choices = choices

    def generateCode( self, writer: Writer ):
        for choice in self._choices:
            writer( f"""if ( s == "{choice}" ) {{""" )
            with indent(writer):
                writer( f"""code = {writer.code(choice)};""" )
                writer.fields( choice )
                writer( f"""return true;""")
            writer( f"""}}""" )
        writer( f"""return false;""" )

### makeTree builds the decision tree for a list of words.

def findEvenDivisions( chars ):
    L = len( chars )
    N = len( chars ) // 2
    for delta in range( 0, N ):
        if N + delta + 1 < L:
            if chars[ N + delta ] != chars[ N + delta + 1 ]:
                return ( delta, chars[N + delta ], chars[N + delta + 1] )
        if N - delta - 1 >= 0:
            if chars[ N - delta ] != chars[ N - delta - 1 ]:
                return ( -delta, chars[N - delta - 1], chars[N - delta]  )
    return None

def findEvenDivisionsFor( words, i ):
    chars = [ s[i] if len(s) > i else "\0" for s in words ]
    chars = sorted( chars )
    return findEvenDivisions( chars )

def chooseBestEvenDivision( words ):
    W = max( 0, *map( len, words ) )
    best_delta = None
    best_i = None
    best_low_char = None
    for i in range( 0, W ):
        evenDivision = findEvenDivisionsFor( words, i )
        if evenDivision is not None:
            ( delta, low_char, _ ) = evenDivision
            # print( 'SOFAR', 'i =', i, 'best_delta =', best_delta, 'best_low_char = ', best_low_char, 'delta =', delta, 'i =', i, 'low_char =', low_char )
            if best_delta is None or ( abs( delta ) < abs( best_delta ) ):
                best_delta = delta
                best_i = i
                best_low_char = low_char
                # print( 'i = ', i, 'best_delta =', best_delta, 'delta =', delta, 'i =', i, 'low_char =', low_char )
    return ( best_i, best_low_char )
    
def splitWords( words, i, low_char ):
    left = []
    right = []
    for word in words:
        if len( word ) <= i:
            left.append( word )
        elif word[i] <= low_char:
            left.append( word )
        else:
            right.append( word )
    return ( left, right )
    
def makeTree( words, *, L=0 ):
    if len( words ) <= 1:
        return OneOf( words )
    else:
        ( i, low_char ) = chooseBestEvenDivision( words )
        if i is None:
            return OneOf( words )
        else:
            ( left, right ) = splitWords( words, i, low_char )
            if len( left ) == 0 or len( right ) == 0:
                return OneOf( words )
            else:
                return IfThenElse( ( i, low_char ), L, makeTree( left, L=L ), makeTree( right, L=L ) )

def altMakeTree( words ):
    if len( words ) == 0:
        return makeTree( [], L=0 )
    else:
        words_by_len = {}
        for w in words:
            L = len( w )
            if L not in words_by_len:
                words_by_len[L] = []
            words_by_len[L].append( w )
        return SwitchOnLength( { L: makeTree( words, L=L ) for ( L, words ) in words_by_len.items() } )   


### CodeGenerator generates the code for the decision tree.

class CodeGenerator:

    def __init__( self, jdata ):
        self._jdata = jdata
        self._jtemplate = jdata['template']
        words: List[ str ] = list( jdata['map'].keys() )
        self._tree = altMakeTree( words )
        self._include_empty = '' in words

    def makeWriter( self, file ):
        fields = tuple( field['name'] for field in self._jdata['fields'] )
        codes_classname = self.name_of_CodeClass()
        return Writer( file, codes_classname, self._jdata['map'], fields )

    def defaultCode( self ):
        code_class = self.name_of_CodeClass()
        code = self._jdata['default-code']
        return f"{code_class}::{code}"

    def name_of_CodeClass( self ):
        return self._jtemplate['class-names']['codes']

    def name_of_KeyClass( self ):
        return self._jtemplate['class-names']['keys']
    
    def name_FromKeyToCodeFunction( self ):
        try:
            return self._jtemplate['function-names']['from-key-to-code']
        except:
            return None

    def name_of_FromCodeToKeyFunction( self ):
        try:
            return self._jdata['template']['function-names']['from-code-to-key']
        except:
            return None

    def name_LookupFunction( self ):
        try:
            return self._jdata['template']['function-names']['lookup']
        except:
            return "lookupCode"
        
    def name_HasCodeFunction( self ):
        try:
            return self._jdata['template']['function-names']['has-code']
        except:
            return None

    def generateCode( self ):
        jtemplate = self._jtemplate
        hfile = jtemplate['header']['file']
        sfile = jtemplate['source']['file']
        with open( hfile, 'w' ) as f:
            self.generateHeader( self.makeWriter(f) )
        with open( sfile, 'w' ) as f:
            self.generateSource( self.makeWriter(f) )

    def generateKeysClass( self, writer ):
        keys_classname = self.name_of_KeyClass()
        writer( f"""class {keys_classname} {{""" )
        with indent(writer):
            for k, v in self._jdata['map'].items():
                name = v['key']
                writer( f"""static const std::string_view {name};""" )
        writer( "}; // class" )
        writer()

    def generateCodeClass( self, writer ):
        codes_classname = self.name_of_CodeClass()
        writer( f"""enum class {codes_classname} {{""" )
        with indent(writer):
            for e in self._jdata['enums']:
                writer( f"""{e}, """ )
            for k, v in self._jdata['map'].items():
                code = v['code']
                writer( f"""{code}, """ )      
        writer( "}; // enum" )
        writer()

    def generateInnerHeader( self, writer ):
        self.generateKeysClass( writer )
        self.generateCodeClass( writer )

        codes_classname = self.name_of_CodeClass()
        code_class = self.name_of_CodeClass()

        lookup = self.name_LookupFunction()
        extra_fields = ''.join( f""", {field['type']} & {field['name']}""" for field in self._jdata['fields'] )

        writer( f"""bool {lookup}( const std::string & s, {code_class} & code {extra_fields});""" )
        writer()
        
        if code_to_string := self.name_of_FromCodeToKeyFunction():
            writer( f"""const char * {code_to_string}( {codes_classname} code );""" )
            writer()

        if string_to_code := self.name_FromKeyToCodeFunction():
            writer( f"""{codes_classname} {string_to_code}( const std::string & s );""" )
            writer()

        if has_code := self.name_HasCodeFunction():
            writer( f"""bool {has_code}( const std::string & s );""" )
            writer()
        

    def generateHeader( self, writer ):
        jtemplate = self._jtemplate
        writer( f"""#ifndef {jtemplate['include-once']}""" )
        writer( f"""#define {jtemplate['include-once']}""" )
        writer()
        writer( """#include <string>""" )
        writer()
        for i in jtemplate['header']['include']:
            writer( i )
        writer()
        writer( f"""namespace {jtemplate['namespace']} {{""" )
        writer()
        
        self.generateInnerHeader( writer )
        
        writer()
        writer( "} // namespace" )
        writer( """#endif""" )

    def generateSource( self, writer ):
        jtemplate = self._jtemplate
        writer( """#include <string>""" )
        writer()
        writer( f"""#include "{jtemplate['header']['file']}" """ )
        writer()
        for i in jtemplate['source']['include']:
            writer( i )
        writer()
        writer( f"""namespace {jtemplate['namespace']} {{""" )
        writer()

        self.generateInnerSource( writer )

        writer( "} // namespace" )

    def generateNameDefinitions( self, writer ):
        keys_classname = self.name_of_KeyClass()
        for k, v in self._jdata['map'].items():
            mod_name = v['key']
            writer( f"""const std::string_view {keys_classname}::{mod_name} {{ \"{k}\" }};""" )      
        writer()

    def generateLookupFunction( self, writer ):
        if lookup := self.name_LookupFunction():
            code_class = self.name_of_CodeClass()
            extra_fields = ''.join( f""", {field['type']} & {field['name']}""" for field in self._jdata['fields'] )
            writer( f"""bool {lookup}( const std::string & s, {code_class} & code {extra_fields}) {{""" )
            with indent(writer):
                writer( """size_t len = s.length();""" )
                dc = self.defaultCode()
                writer( f"""code = {dc};""" )
                for extra in self._jdata['fields']:
                    writer( f"""{extra['name']} = {extra['default']};""" )
                if self._include_empty:
                    writer( """if ( len == 0 )""" )
                    with indent(writer):
                        writer( """return true;""" )
                    writer( f"""return {self._include_empty};""" )
                self._tree.generateCode( writer )
            writer( """}""" )
            writer()

    def generateCodeToStringFunction( self, writer ):
        if code_to_string := self.name_of_FromCodeToKeyFunction():
            codes_classname = self.name_of_CodeClass()
            writer( f"""const char * {code_to_string}( {codes_classname} code )""" )
            writer( """{""", )
            with indent(writer):
                writer( """switch ( code ) {""" )
                with indent(writer):
                    for k, v in self._jdata['map'].items():
                        code = v['code']
                        writer( f"""case {codes_classname}::{code}: return \"{k}\";""" )
                    writer( f"""default: return nullptr;""" )
                writer( """}""" )
            writer( """}""" )
            writer()

    def generateKeyToCodeFunction( self, writer ):
        if key_to_code := self.name_FromKeyToCodeFunction():
            lookup = self.name_LookupFunction()
            codes_classname = self.name_of_CodeClass()
            writer( f"""{codes_classname} {key_to_code}( const std::string & key )""" )
            writer( """{""", )
            with indent(writer):
                writer( f"""{codes_classname} code;""" )
                for extra in self._jdata['fields']:
                    writer( f"""{extra['type']} {extra['name']};""" )
                extra_fields = ''.join( f""", {field['name']}""" for field in self._jdata['fields'] )
                writer( f"""{lookup}( key, code {extra_fields});""" )
                writer( """return code;""" )
            writer( """}""" )
            writer()

    def generateHasCodeFunction( self, writer ):
        if has_code := self.name_HasCodeFunction():
            lookup = self.name_LookupFunction()
            writer( f"""bool {has_code}( const std::string & key )""" )
            writer( """{""", )
            with indent(writer):
                writer( f"""{self.name_of_CodeClass()} code;""" )
                for extra in self._jdata['fields']:
                    writer( f"""{extra['type']} {extra['name']};""" )
                extra_fields = ''.join( f""", {field['name']}""" for field in self._jdata['fields'] )
                writer( f"""return {lookup}( key, code {extra_fields});""" )
            writer( """}""" )
            writer()

    def generateInnerSource( self, writer ):
        self.generateNameDefinitions( writer )
        self.generateLookupFunction( writer )
        self.generateCodeToStringFunction( writer )
        self.generateKeyToCodeFunction( writer )
        self.generateHasCodeFunction( writer )


def main(filename):
    with open( filename, 'r' ) as f:
        jdata = json.load( f )
        CodeGenerator( jdata ).generateCode()

if __name__ == "__main__":
    main(sys.argv[1])
