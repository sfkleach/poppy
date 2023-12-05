#include <iostream>

#include <string>
bool inWords( std::string s) {
    size_t len = s.length();
    if ( s[0] <= ']' ) {
        if ( s[0] <= '/' ) {
            if ( s[0] <= '%' ) {
                if ( len <= 1 || s[1] <= '<' ) {
                    if ( len <= 1 || s[1] <= ')' ) {
                        if ( s[0] <= '!' ) {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "!" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 2 || s[2] <= '\0' ) {
                                    if ( s == "!!" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "!!!" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( s[0] <= '$' ) {
                                if ( s == "$" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "%)" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 2 || s[2] <= '=' ) {
                            if ( len <= 2 || s[2] <= '\0' ) {
                                if ( s == "!<" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 3 || s[3] <= '\0' ) {
                                    if ( s == "!<=" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "!<=>" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( s == "!<>" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( len <= 1 || s[1] <= '=' ) {
                        if ( len <= 2 || s[2] <= '=' ) {
                            if ( len <= 2 || s[2] <= '\0' ) {
                                if ( s == "!=" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "!==" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "!=^=" ) {
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= '!' ) {
                            if ( len <= 2 || s[2] <= '\0' ) {
                                if ( s == "!>" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "!>=" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 1 || s[1] <= ']' ) {
                                if ( s == "%]" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "%}" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    }
                }
            } else {
                if ( s[0] <= '+' ) {
                    if ( s[0] <= ')' ) {
                        if ( s[0] <= '&' ) {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "&" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "&&" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s[0] <= '(' ) {
                                if ( len <= 1 || s[1] <= '\0' ) {
                                    if ( s == "(" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "(%" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == ")" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= '*' ) {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "*" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "**" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "+" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "++" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    }
                } else {
                    if ( s[0] <= '.' ) {
                        if ( s[0] <= '-' ) {
                            if ( s[0] <= ',' ) {
                                if ( s == "," ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 1 || s[1] <= '\0' ) {
                                    if ( s == "-" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "->" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "." ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "..." ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 1 || s[1] <= '/' ) {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "/" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "//" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "/>" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            }
        } else {
            if ( len <= 1 || s[1] <= ':' ) {
                if ( len <= 1 || s[1] <= '%' ) {
                    if ( s[0] <= '?' ) {
                        if ( s[0] <= '=' ) {
                            if ( s[0] <= '<' ) {
                                if ( s[0] <= ';' ) {
                                    if ( s[0] <= ':' ) {
                                        if ( s == ":" ) {
                                            return true;
                                        }
                                        return false;
                                    } else {
                                        if ( s == ";" ) {
                                            return true;
                                        }
                                        return false;
                                    }
                                } else {
                                    if ( s == "<" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "=" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s[0] <= '>' ) {
                                if ( s == ">" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "?!" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= '[' ) {
                            if ( s[0] <= '@' ) {
                                if ( s == "@" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 1 || s[1] <= '\0' ) {
                                    if ( s == "[" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "[%" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( s == "]" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= '<' ) {
                        if ( s[0] <= ':' ) {
                            if ( len <= 1 || s[1] <= '-' ) {
                                if ( s == ":-" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "::=" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 1 || s[1] <= '-' ) {
                                if ( s == "<-" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 2 || s[2] <= '\0' ) {
                                    if ( s == "</" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "</>" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        }
                    } else {
                        if ( s[0] <= '=' ) {
                            if ( len <= 2 || s[2] <= '\0' ) {
                                if ( s == "=:" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "=::" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == ">->" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            } else {
                if ( len <= 2 || s[2] <= '\0' ) {
                    if ( s[0] <= '<' ) {
                        if ( s[0] <= ';' ) {
                            if ( s[0] <= ':' ) {
                                if ( s == ":=" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == ";;" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 1 || s[1] <= '=' ) {
                                if ( s == "<=" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "<>" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= '>' ) {
                            if ( s[0] <= '=' ) {
                                if ( len <= 1 || s[1] <= '=' ) {
                                    if ( s == "==" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "=>" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == ">=" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "??" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= 'F' ) {
                        if ( s[0] <= '=' ) {
                            if ( s[0] <= '<' ) {
                                if ( s == "<=>" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 1 || s[1] <= '>' ) {
                                    if ( s == "=>>" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "=^=" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( s == "False" ) {
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= 'N' ) {
                            if ( s == "None" ) {
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "True" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            }
        }
    } else {
        if ( s[0] <= 'f' ) {
            if ( len <= 2 || s[2] <= 'd' ) {
                if ( len <= 3 || s[3] <= 'd' ) {
                    if ( s[0] <= 'd' ) {
                        if ( s[0] <= 'a' ) {
                            if ( s[0] <= '_' ) {
                                if ( s[0] <= '^' ) {
                                    if ( s == "^^" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "_" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "and" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s[0] <= 'b' ) {
                                if ( s == "by" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "do" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= 'e' ) {
                            if ( len <= 3 || s[3] <= '\0' ) {
                                if ( s == "end" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "enddefine" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "fn" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( len <= 3 || s[3] <= 'r' ) {
                        if ( len <= 3 || s[3] <= 'i' ) {
                            if ( len <= 3 || s[3] <= 'f' ) {
                                if ( len <= 4 || s[4] <= 'n' ) {
                                    if ( s == "endfn" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "endfor" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "endif" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 3 || s[3] <= 'p' ) {
                                if ( s == "endpackage" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "endrecordclass" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 3 || s[3] <= 't' ) {
                            if ( len <= 3 || s[3] <= 's' ) {
                                if ( s == "endswitch" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 5 || s[5] <= 'a' ) {
                                    if ( s == "endtransaction" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "endtry" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( s == "endunless" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            } else {
                if ( s[0] <= 'd' ) {
                    if ( s[0] <= 'c' ) {
                        if ( s[0] <= 'a' ) {
                            if ( s == "also" ) {
                                return true;
                            }
                            return false;
                        } else {
                            if ( len <= 2 || s[2] <= 's' ) {
                                if ( s == "case" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "catch" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 1 || s[1] <= 'e' ) {
                            if ( len <= 3 || s[3] <= 'a' ) {
                                if ( s == "default" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "define" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 3 || s[3] <= '\0' ) {
                                if ( s == "div" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "divmod" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    }
                } else {
                    if ( len <= 1 || s[1] <= 'l' ) {
                        if ( s[0] <= 'e' ) {
                            if ( len <= 4 || s[4] <= 'i' ) {
                                if ( len <= 4 || s[4] <= '\0' ) {
                                    if ( s == "else" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "elseif" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "elseunless" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 2 || s[2] <= 'e' ) {
                                if ( s == "field" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "finally" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 1 || s[1] <= 'r' ) {
                            if ( len <= 1 || s[1] <= 'o' ) {
                                if ( s == "for" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "from" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "function" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            }
        } else {
            if ( len <= 1 || s[1] <= 'h' ) {
                if ( s[0] <= 'v' ) {
                    if ( s[0] <= 'r' ) {
                        if ( s[0] <= 'p' ) {
                            if ( s[0] <= 'i' ) {
                                if ( s == "if" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "package" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 2 || s[2] <= 'c' ) {
                                if ( s == "recordclass" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "return" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= 't' ) {
                            if ( len <= 2 || s[2] <= 'e' ) {
                                if ( s == "then" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "throw" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 2 || s[2] <= 'l' ) {
                                if ( s == "val" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "var" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    }
                } else {
                    if ( s[0] <= '{' ) {
                        if ( s[0] <= 'w' ) {
                            if ( len <= 2 || s[2] <= 'e' ) {
                                if ( s == "where" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "while" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 1 || s[1] <= '\0' ) {
                                if ( s == "{" ) {
                                    return true;
                                }
                                if ( s == "{" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "{%" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= '|' ) {
                            if ( s == "|" ) {
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "}" ) {
                                return true;
                            }
                            if ( s == "}" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            } else {
                if ( s[0] <= 's' ) {
                    if ( s[0] <= 'o' ) {
                        if ( s[0] <= 'm' ) {
                            if ( s[0] <= 'i' ) {
                                if ( len <= 1 || s[1] <= 'm' ) {
                                    if ( s == "import" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "in" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "mod" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s[0] <= 'n' ) {
                                if ( s == "not" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "or" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 1 || s[1] <= 'l' ) {
                            if ( len <= 1 || s[1] <= 'k' ) {
                                if ( s == "skip" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "slot" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "switch" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= 't' ) {
                        if ( len <= 1 || s[1] <= 'o' ) {
                            if ( s == "to" ) {
                                return true;
                            }
                            return false;
                        } else {
                            if ( len <= 2 || s[2] <= 'a' ) {
                                if ( s == "transaction" ) {
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "try" ) {
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= 'w' ) {
                            if ( s[0] <= 'u' ) {
                                if ( len <= 2 || s[2] <= 'l' ) {
                                    if ( s == "unless" ) {
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "until" ) {
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "with" ) {
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "||" ) {
                                return true;
                            }
                            return false;
                        }
                    }
                }
            }
        }
    }
}



int main(int argc, char **argv) {
    std::cout << "checking " << argv[1] << std::endl;
    if ( inWords( argv[1] ) ) {
        std::cout << "true" << std::endl;
    } else {
        std::cout << "false" << std::endl;
    }
    return EXIT_SUCCESS;
}