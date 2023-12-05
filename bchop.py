from typing import List
from abc import ABC, abstractmethod
import json


### Nodes represent the structure of the if-then-else decision tree.

class Writer:

    def __init__( self, file ):
        self._file = file
        self._indent = 0

    def indent( self ):
        self._indent += 1

    def dedent( self ):
        self._indent -= 1

    def writeLine( self, s ):
        for i in range( 0, self._indent ):
            print( '    ', end = "", file = self._file )
        print( s, file = self._file )


    

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


class BChop:

    def __init__( self, jdata ):
        self._jdata = jdata


class CodeGenerator:

    def __init__( self, jdata ):
        self._jdata = jdata
        words: List[ str ] = list( jdata['map'].keys() )
        self._tree = altMakeTree( words )
        self._include_empty = '' in words

    def generateCode( self ):
        jtemplate = self._jdata['template']
        hfile = jtemplate['header']
        sfile = jtemplate['source']
        with open( hfile, 'w' ) as f:
            self.generateHeader( f )
        with open( sfile, 'w' ) as f:
            self.generateSource( f )

    def generateHeader( self, file ):
        jtemplate = self._jdata['template']
        print( f"""#ifndef {jtemplate['include-once']}""", file = file )
        print( f"""#define {jtemplate['include-once']}""", file = file )
        print( file = file )
        print( """#include <string>""", file = file )
        print( file = file )
        print( f"""namespace {jtemplate['namespace']} {{""", file = file )
        print( file = file )
        
        jnames = jtemplate['names']
        names_classname = jnames['class']
        names_format = jnames['format'] if 'format' in jnames else '{0}'
        print( f"""class {names_classname} {{""", file = file )
        for k, v in self._jdata['map'].items():
            name = names_format.format( v['name'] )
            print( f"""    static const std::string_view {name};""", file = file )      
        print( "}; // class", file = file )
        print( file = file )
        
        jcodes = jtemplate['codes']
        codes_classname = jcodes['class']
        codes_format = jcodes['format'] if 'format' in jcodes else '{0}'
        print( f"""enum class {codes_classname} {{""", file = file )
        for e in self._jdata['enums']:
            print( f"""    {codes_format.format(e)}, """, file = file )
        for k, v in self._jdata['map'].items():
            code = codes_format.format( v['code'] )
            print( f"""    {code}, """, file = file )      
        print( "}; // enum", file = file )
        print( file = file )

        print( """bool stringToCode( const std::string & s );""", file = file )
        print( file = file )
        
        print( f"""const char * codeToString( {codes_classname} code );""", file = file )
        print( file = file )
        
        print( "} // namespace", file = file )
        print( """#endif""", file = file )

    def generateSource( self, file ):
        jtemplate = self._jdata['template']
        print( """#include <string>""", file = file )
        print( file = file )
        print( f"""#include "{jtemplate['header']}" """, file = file )
        print( file = file )
        print( f"""namespace {jtemplate['namespace']} {{""", file = file )
        print( file = file )

        jnames = jtemplate['names']
        names_classname = jnames['class']
        names_format = jnames['format'] if 'format' in jnames else '{0}'
        for k, v in self._jdata['map'].items():
            name = names_format.format( v['name'] )
            print( f"""const std::string_view {names_classname}::{name} {{ \"{k}\" }};""", file = file )      
        print( file = file )

        print( """bool stringToCode( const std::string & s ) {""", file = file )
        print( """    size_t len = s.length();""", file = file )
        if self._include_empty:
            print( """    if ( len == 0 )""", file = file )
            print( f"""        return {self._include_empty};""", file = file )
        self._tree.generateCode( indent = 1, file = file )
        print( """}""", file = file )
        print( file = file )

        jcodes = jtemplate['codes']
        codes_classname = jcodes['class']  
        codes_format = jcodes['format'] if 'format' in jcodes else '{0}'
        print( f"""const char * codeToString( {codes_classname} code )""", file = file )
        print( """{""", file = file )
        print( """    switch ( code ) {""", file = file )
        for k, v in self._jdata['map'].items():
            code = codes_format.format( v['code'] )
            print( f"""        case {codes_classname}::{code}: return \"{k}\";""", file = file )
        default_name = self._jdata['default']['name']
        print( f"""        default: return \"{default_name}\";""", file = file )
        print( """    }""", file = file )
        print( """}""", file = file )

        print( "} // namespace", file = file )


def main():
    with open( 'itemattrs.json', 'r' ) as f:
        jdata = json.load( f )
        CodeGenerator( jdata ).generateCode()

if __name__ == "__main__":
    main()
