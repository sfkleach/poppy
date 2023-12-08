from typing import List
from abc import ABC, abstractmethod
import json
import sys

### Nodes represent the structure of the if-then-else decision tree.

class Writer:

    def __init__( self, file, map_info ):
        self._file = file
        self._indent = 0
        self._map_info = map_info

    def indent( self ):
        self._indent += 1

    def dedent( self ):
        self._indent -= 1

    def writeLine( self, s ):
        for i in range( 0, self._indent ):
            print( '    ', end = "", file = self._file )
        print( s, file = self._file )

    def code( self, x ):
        return self._map_info[x]['code']

class Node( ABC ):

    def indent( self, indent: int, *, file,  ):
        for i in range( 0, indent ):
            print( '    ', end = "", file = file )
    
    @abstractmethod
    def generateCode( self, *, file, indent: int = 0 ):
        ...

"""This represents a binary chop at a particular character position."""
class IfThenElse( Node ):
        
    def __init__( self, condition: (int, str), min_len: int, then_part: Node, else_part: Node ):
        self._condition = condition
        self_min_len = min_len
        self._then_part = then_part
        self._else_part = else_part

    def generateCode( self, *, file, indent: int = 0 ):
        ( i, low_char ) = self._condition
        low_char_in_cpp = low_char if low_char != '\0' else '\\0'
        self.indent( indent, file = file )
        if i == 0:
            print( f"""if ( s[{i}] <= '{low_char_in_cpp}' ) {{""", file = file )
        else:
            print( f"""if ( len <= {i} || s[{i}] <= '{low_char_in_cpp}' ) {{""", file = file )
        self._then_part.generateCode( indent = indent + 1, file = file )
        self.indent( indent, file = file )
        print( f"""}} else {{""", file = file )
        self._else_part.generateCode( indent = indent + 1, file = file )
        self.indent( indent, file = file )
        print( f"""}}""", file = file )

"""This represents a switch on the length of the string."""
class SwitchOnLength( Node ):
    
        def __init__( self, cases: dict ):
            self._cases = cases
    
        def generateCode( self, *, file, indent: int = 0 ):
            self.indent( indent, file = file )
            print( f"""switch ( len ) {{""", file = file )
            for k, v in self._cases.items():
                self.indent( indent + 1, file = file )
                print( f"""case {k}: {{""", file = file )
                v.generateCode( indent = indent + 2, file = file )
                self.indent( indent + 1, file = file )
                print( f"""}} break;""", file = file )
            self.indent( indent + 1, file = file )
            print( f"""default: return false;""", file = file )
            self.indent( indent, file = file )
            print( f"""}}""", file = file )

"""
If this is a single word, we should test for this. Otherwise we failed to 
find a good decision, just do a linear search.
"""
class OneOf( Node ):

    def __init__( self, choices: List[ str ] ):
        self._choices = choices

    def generateCode( self, *, file, indent: int = 0 ):
        for choice in self._choices:
            self.indent( indent, file = file )
            print( f"""if ( s == "{choice}" ) {{""", file = file )
            self.indent( indent, file = file )
            print( f"""    return true;""", file = file )
            self.indent( indent, file = file )
            print( f"""}}""", file = file )
        self.indent( indent, file = file )
        print( f"""return false;""", file = file )

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
        return Writer( file, self._jdata['map'] )

    def defaultCode( self ):
        code_class = self.name_of_CodeClass()
        code = self.modCode( self._jdata['defaults']['code'] )
        return f"{code_class}::{code}"

    def name_of_CodeClass( self ):
        return self._jtemplate['codes']['class']
    
    def modCode( self, code ):
        return self._jtemplate['codes']['format'].format( code )

    def name_of_KeyClass( self ):
        return self._jtemplate['keys']['class']
    
    def modKey( self, name ):
        return self._jtemplate['keys']['format'].format( name )

    def name_FromKeyToCodeFunction( self ):
        try:
            return self._jtemplate['function-names']['from-string-to-code']
        except:
            return None

    def name_of_FromCodeToKeyFunction( self ):
        try:
            return self._jdata['template']['function-names']['code-to-string']
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

    def generateKeysClass( self, file ):
        keys_classname = self.name_of_KeyClass()
        print( f"""class {keys_classname} {{""", file = file )
        for k, v in self._jdata['map'].items():
            name = self.modKey( v['key'] )
            print( f"""    static const std::string_view {name};""", file = file )      
        print( "}; // class", file = file )
        print( file = file )

    def generateCodeClass( self, file ):
        codes_classname = self.name_of_CodeClass()
        print( f"""enum class {codes_classname} {{""", file = file )
        for e in self._jdata['enums']:
            code = self.modCode(e)
            print( f"""    {code}, """, file = file )
        for k, v in self._jdata['map'].items():
            code = self.modCode( v['code'] )
            print( f"""    {code}, """, file = file )      
        print( "}; // enum", file = file )
        print( file = file )

    def generateInnerHeader( self, file ):
        self.generateKeysClass( file )
        self.generateCodeClass( file )

        codes_classname = self.name_of_CodeClass()
        code_class = self.name_of_CodeClass()

        lookup = self.name_LookupFunction()
        print( f"""bool {lookup}( const std::string & s, {code_class} & code );""", file = file )
        print( file = file )
        
        if code_to_string := self.name_of_FromCodeToKeyFunction():
            print( f"""const char * {code_to_string}( {codes_classname} code );""", file = file )
            print( file = file )

        if string_to_code := self.name_FromKeyToCodeFunction():
            print( f"""{codes_classname} {string_to_code}( const std::string & s );""", file = file )
            print( file = file )

        if has_code := self.name_HasCodeFunction():
            print( f"""bool {has_code}( const std::string & s );""", file = file )
            print( file = file )
        

    def generateHeader( self, file ):
        jtemplate = self._jtemplate
        print( f"""#ifndef {jtemplate['include-once']}""", file = file )
        print( f"""#define {jtemplate['include-once']}""", file = file )
        print( file = file )
        print( """#include <string>""", file = file )
        print( file = file )
        for i in jtemplate['header']['include']:
            print( i, file = file )
        print( file = file )
        print( f"""namespace {jtemplate['namespace']} {{""", file = file )
        print( file = file )
        
        self.generateInnerHeader( file )
        
        print( "} // namespace", file = file )
        print( """#endif""", file = file )

    def generateSource( self, file ):
        jtemplate = self._jtemplate
        print( """#include <string>""", file = file )
        print( file = file )
        print( f"""#include "{jtemplate['header']['file']}" """, file = file )
        print( file = file )
        for i in jtemplate['source']['include']:
            print( i, file = file )
        print( file = file )
        print( f"""namespace {jtemplate['namespace']} {{""", file = file )
        print( file = file )

        self.generateInnerSource( file )

        print( "} // namespace", file = file )

    def generateNameDefinitions( self, file ):
        keys_classname = self.name_of_KeyClass()
        for k, v in self._jdata['map'].items():
            mod_name = self.modKey( v['key'] )
            print( f"""const std::string_view {keys_classname}::{mod_name} {{ \"{k}\" }};""", file = file )      
        print( file = file )

    def generateLookupFunction( self, file ):
        if lookup := self.name_LookupFunction():
            code_class = self.name_of_CodeClass()
            print( f"""bool {lookup}( const std::string & s, {code_class} & code ) {{""", file = file )
            print( """    size_t len = s.length();""", file = file )
            dc = self.defaultCode()
            print( f"""    code = {dc};""", file = file )
            if self._include_empty:
                print( """    if ( len == 0 )""", file = file )
                print( f"""        return {self._include_empty};""", file = file )
            self._tree.generateCode( indent = 1, file = file )
            print( """}""", file = file )
            print( file = file )

    def generateCodeToStringFunction( self, file ):
        codes_classname = self.name_of_CodeClass()
        if code_to_string := self.name_of_FromCodeToKeyFunction():
            print( f"""const char * {code_to_string}( {codes_classname} code )""", file = file )
            print( """{""", file = file )
            print( """    switch ( code ) {""", file = file )
            for k, v in self._jdata['map'].items():
                code = self.modCode( v['code'] )
                print( f"""        case {codes_classname}::{code}: return \"{k}\";""", file = file )
            default_name = self._jdata['default']['name']
            print( f"""        default: return \"{default_name}\";""", file = file )
            print( """    }""", file = file )
            print( """}""", file = file )

    def generateInnerSource( self, file ):
        self.generateNameDefinitions( file )
        self.generateLookupFunction( file )
        self.generateCodeToStringFunction( file )


def main(filename):
    with open( filename, 'r' ) as f:
        jdata = json.load( f )
        writer = Writer( std., jdata['map'] )
        CodeGenerator( jdata ).generateCode()

if __name__ == "__main__":
    main(sys.argv[1])
