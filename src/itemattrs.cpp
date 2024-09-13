#include <string>

#include "itemattrs.hpp" 


namespace poppy {

const std::string_view ItemKey::KW_ADD { "+" };
const std::string_view ItemKey::KW_AND { "and" };
const std::string_view ItemKey::KW_APPEND { "++" };
const std::string_view ItemKey::KW_ARROW_RIGHT { "->" };
const std::string_view ItemKey::KW_BY { "by" };
const std::string_view ItemKey::KW_CASE { "case" };
const std::string_view ItemKey::KW_CATCH { "catch" };
const std::string_view ItemKey::KW_BRACE_CLOSE { "}" };
const std::string_view ItemKey::KW_LIST_CLOSE { "]" };
const std::string_view ItemKey::KW_COLON { ":" };
const std::string_view ItemKey::KW_COMMA { "," };
const std::string_view ItemKey::KW_PAREN_CLOSE { ")" };
const std::string_view ItemKey::KW_FAT_PAREN_CLOSE { "%)" };
const std::string_view ItemKey::KW_DEFINE { "define" };
const std::string_view ItemKey::KW_DISCARD { "_" };
const std::string_view ItemKey::KW_2_SEMICOLON { ";;" };
const std::string_view ItemKey::KW_DEFAULT { "default" };
const std::string_view ItemKey::KW_DIV { "div" };
const std::string_view ItemKey::KW_DIV_MOD { "divmod" };
const std::string_view ItemKey::KW_DO { "do" };
const std::string_view ItemKey::KW_INFIX_APPLY_1 { "dot" };
const std::string_view ItemKey::KW_ELSE { "else" };
const std::string_view ItemKey::KW_ELSEIF { "elseif" };
const std::string_view ItemKey::KW_ELSEUNLESS { "elseunless" };
const std::string_view ItemKey::KW_END { "end" };
const std::string_view ItemKey::KW_ENDDEFINE { "enddefine" };
const std::string_view ItemKey::KW_ENDDO { "enddo" };
const std::string_view ItemKey::KW_ENDLAMBDA { "endfn" };
const std::string_view ItemKey::KW_ENDFOR { "endfor" };
const std::string_view ItemKey::KW_ENDIF { "endif" };
const std::string_view ItemKey::KW_ENDSWITCH { "endswitch" };
const std::string_view ItemKey::KW_ENDTRY { "endtry" };
const std::string_view ItemKey::KW_ENDUNLESS { "endunless" };
const std::string_view ItemKey::KW_DOLLAR { "$" };
const std::string_view ItemKey::KW_EQUAL { "=" };
const std::string_view ItemKey::KW_EXACT_EQUAL { "==" };
const std::string_view ItemKey::KW_EXPLODE { "..." };
const std::string_view ItemKey::KW_FALSE { "False" };
const std::string_view ItemKey::KW_FAT_BRACE_CLOSE { "%}" };
const std::string_view ItemKey::KW_FAT_BRACKET_CLOSE { "%]" };
const std::string_view ItemKey::KW_FINALLY { "finally" };
const std::string_view ItemKey::KW_LAMBDA { "fn" };
const std::string_view ItemKey::KW_FOR { "for" };
const std::string_view ItemKey::KW_FROM { "from" };
const std::string_view ItemKey::KW_GT { ">" };
const std::string_view ItemKey::KW_GTE { ">=" };
const std::string_view ItemKey::KW_IF { "if" };
const std::string_view ItemKey::KW_IN { "in" };
const std::string_view ItemKey::KW_LT { "<" };
const std::string_view ItemKey::KW_LTE { "<=" };
const std::string_view ItemKey::KW_LTGT { "<>" };
const std::string_view ItemKey::KW_LTEGT { "<=>" };
const std::string_view ItemKey::KW_NOT_EQUAL { "!=" };
const std::string_view ItemKey::KW_NOT_EXACT_EQUAL { "!==" };
const std::string_view ItemKey::KW_NOT_LT { "!<" };
const std::string_view ItemKey::KW_NOT_LTE { "!<=" };
const std::string_view ItemKey::KW_NOT_GT { "!>" };
const std::string_view ItemKey::KW_NOT_GTE { "!>=" };
const std::string_view ItemKey::KW_NOT_LTGT { "!<>" };
const std::string_view ItemKey::KW_NOT_LTEGT { "!<=>" };
const std::string_view ItemKey::KW_LT_SLASH { "</" };
const std::string_view ItemKey::KW_MAPLET { "=>" };
const std::string_view ItemKey::KW_MUL { "*" };
const std::string_view ItemKey::KW_NOT { "not" };
const std::string_view ItemKey::KW_BRACE_OPEN { "{" };
const std::string_view ItemKey::KW_LIST_OPEN { "[" };
const std::string_view ItemKey::KW_PAREN_OPEN { "(" };
const std::string_view ItemKey::KW_FAT_BRACE_OPEN { "{%" };
const std::string_view ItemKey::KW_FAT_BRACKET_OPEN { "[%" };
const std::string_view ItemKey::KW_FAT_PAREN_OPEN { "(%" };
const std::string_view ItemKey::KW_MOD { "mod" };
const std::string_view ItemKey::KW_NONE { "None" };
const std::string_view ItemKey::KW_OR { "or" };
const std::string_view ItemKey::KW_PANIC { "!!!" };
const std::string_view ItemKey::KW_POW { "**" };
const std::string_view ItemKey::KW_RETURN { "return" };
const std::string_view ItemKey::KW_SEMICOLON { ";" };
const std::string_view ItemKey::KW_SKIP { "skip" };
const std::string_view ItemKey::KW_SLASH_GT { "/>" };
const std::string_view ItemKey::KW_SUB { "-" };
const std::string_view ItemKey::KW_SWITCH { "switch" };
const std::string_view ItemKey::KW_TERMIN { "Termin" };
const std::string_view ItemKey::KW_THEN { "then" };
const std::string_view ItemKey::KW_TRUE { "True" };
const std::string_view ItemKey::KW_TO { "to" };
const std::string_view ItemKey::KW_UNLESS { "unless" };
const std::string_view ItemKey::KW_UNTIL { "until" };
const std::string_view ItemKey::KW_VAL { "val" };
const std::string_view ItemKey::KW_VAR { "var" };
const std::string_view ItemKey::KW_WHERE { "where" };
const std::string_view ItemKey::KW_WHILE { "while" };

bool lookupItemCode( const std::string & s, ItemCode & code ) {
    size_t len = s.length();
    code = ItemCode::unknown_code;
    switch ( len ) {
        case 1: {
            if ( s[0] <= ';' ) {
                if ( s[0] <= '+' ) {
                    if ( s[0] <= ')' ) {
                        if ( s[0] <= '(' ) {
                            if ( s[0] <= '$' ) {
                                if ( s == "$" ) {
                                    code = ItemCode::envvar_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "(" ) {
                                    code = ItemCode::oparen_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == ")" ) {
                                code = ItemCode::cparen_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= '*' ) {
                            if ( s == "*" ) {
                                code = ItemCode::mul_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "+" ) {
                                code = ItemCode::add_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= ':' ) {
                        if ( s[0] <= '-' ) {
                            if ( s[0] <= ',' ) {
                                if ( s == "," ) {
                                    code = ItemCode::comma_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "-" ) {
                                    code = ItemCode::sub_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == ":" ) {
                                code = ItemCode::colon_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s == ";" ) {
                            code = ItemCode::semi_code;
                            return true;
                        }
                        return false;
                    }
                }
            } else {
                if ( s[0] <= ']' ) {
                    if ( s[0] <= '>' ) {
                        if ( s[0] <= '=' ) {
                            if ( s[0] <= '<' ) {
                                if ( s == "<" ) {
                                    code = ItemCode::lt_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "=" ) {
                                    code = ItemCode::equal_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == ">" ) {
                                code = ItemCode::gt_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= '[' ) {
                            if ( s == "[" ) {
                                code = ItemCode::obracket_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "]" ) {
                                code = ItemCode::cbracket_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= '{' ) {
                        if ( s[0] <= '_' ) {
                            if ( s == "_" ) {
                                code = ItemCode::Discard_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "{" ) {
                                code = ItemCode::obrace_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s == "}" ) {
                            code = ItemCode::cbrace_code;
                            return true;
                        }
                        return false;
                    }
                }
            }
        } break;
        case 3: {
            if ( s[0] <= 'a' ) {
                if ( s[0] <= '!' ) {
                    if ( len <= 1 || s[1] <= '<' ) {
                        if ( len <= 1 || s[1] <= '!' ) {
                            if ( s == "!!!" ) {
                                code = ItemCode::panic_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( len <= 2 || s[2] <= '=' ) {
                                if ( s == "!<=" ) {
                                    code = ItemCode::not_lte_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "!<>" ) {
                                    code = ItemCode::not_ltgt_code;
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( len <= 1 || s[1] <= '=' ) {
                            if ( s == "!==" ) {
                                code = ItemCode::not_exact_equal_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "!>=" ) {
                                code = ItemCode::not_gte_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= '<' ) {
                        if ( s[0] <= '.' ) {
                            if ( s == "..." ) {
                                code = ItemCode::explode_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "<=>" ) {
                                code = ItemCode::ltegt_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s == "and" ) {
                            code = ItemCode::and_code;
                            return true;
                        }
                        return false;
                    }
                }
            } else {
                if ( s[0] <= 'm' ) {
                    if ( s[0] <= 'e' ) {
                        if ( s[0] <= 'd' ) {
                            if ( len <= 1 || s[1] <= 'i' ) {
                                if ( s == "div" ) {
                                    code = ItemCode::div_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "dot" ) {
                                    code = ItemCode::dot_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "end" ) {
                                code = ItemCode::end_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= 'f' ) {
                            if ( s == "for" ) {
                                code = ItemCode::for_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "mod" ) {
                                code = ItemCode::mod_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= 'n' ) {
                        if ( s == "not" ) {
                            code = ItemCode::not_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( len <= 2 || s[2] <= 'l' ) {
                            if ( s == "val" ) {
                                code = ItemCode::val_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "var" ) {
                                code = ItemCode::var_code;
                                return true;
                            }
                            return false;
                        }
                    }
                }
            }
        } break;
        case 2: {
            if ( len <= 1 || s[1] <= '=' ) {
                if ( s[0] <= ';' ) {
                    if ( s[0] <= '(' ) {
                        if ( s[0] <= '%' ) {
                            if ( s[0] <= '!' ) {
                                if ( len <= 1 || s[1] <= '<' ) {
                                    if ( s == "!<" ) {
                                        code = ItemCode::not_lt_code;
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "!=" ) {
                                        code = ItemCode::not_equal_code;
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "%)" ) {
                                    code = ItemCode::fat_cparen_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "(%" ) {
                                code = ItemCode::fat_oparen_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= '+' ) {
                            if ( s[0] <= '*' ) {
                                if ( s == "**" ) {
                                    code = ItemCode::pow_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "++" ) {
                                    code = ItemCode::append_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == ";;" ) {
                                code = ItemCode::dsemi_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= '>' ) {
                        if ( s[0] <= '=' ) {
                            if ( s[0] <= '<' ) {
                                if ( len <= 1 || s[1] <= '/' ) {
                                    if ( s == "</" ) {
                                        code = ItemCode::ltslash_code;
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "<=" ) {
                                        code = ItemCode::lte_code;
                                        return true;
                                    }
                                    return false;
                                }
                            } else {
                                if ( s == "==" ) {
                                    code = ItemCode::exact_equal_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == ">=" ) {
                                code = ItemCode::gte_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s[0] <= '[' ) {
                            if ( s == "[%" ) {
                                code = ItemCode::fat_obracket_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "{%" ) {
                                code = ItemCode::fat_obrace_code;
                                return true;
                            }
                            return false;
                        }
                    }
                }
            } else {
                if ( s[0] <= 'b' ) {
                    if ( s[0] <= '/' ) {
                        if ( s[0] <= '%' ) {
                            if ( s[0] <= '!' ) {
                                if ( s == "!>" ) {
                                    code = ItemCode::not_gt_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( len <= 1 || s[1] <= ']' ) {
                                    if ( s == "%]" ) {
                                        code = ItemCode::fat_cbracket_code;
                                        return true;
                                    }
                                    return false;
                                } else {
                                    if ( s == "%}" ) {
                                        code = ItemCode::fat_cbrace_code;
                                        return true;
                                    }
                                    return false;
                                }
                            }
                        } else {
                            if ( s[0] <= '-' ) {
                                if ( s == "->" ) {
                                    code = ItemCode::assign_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "/>" ) {
                                    code = ItemCode::slashgt_code;
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= '=' ) {
                            if ( s[0] <= '<' ) {
                                if ( s == "<>" ) {
                                    code = ItemCode::ltgt_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "=>" ) {
                                    code = ItemCode::maplet_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( s == "by" ) {
                                code = ItemCode::by_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= 'i' ) {
                        if ( s[0] <= 'f' ) {
                            if ( s[0] <= 'd' ) {
                                if ( s == "do" ) {
                                    code = ItemCode::do_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "fn" ) {
                                    code = ItemCode::fn_code;
                                    return true;
                                }
                                return false;
                            }
                        } else {
                            if ( len <= 1 || s[1] <= 'f' ) {
                                if ( s == "if" ) {
                                    code = ItemCode::if_code;
                                    return true;
                                }
                                return false;
                            } else {
                                if ( s == "in" ) {
                                    code = ItemCode::in_code;
                                    return true;
                                }
                                return false;
                            }
                        }
                    } else {
                        if ( s[0] <= 'o' ) {
                            if ( s == "or" ) {
                                code = ItemCode::or_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "to" ) {
                                code = ItemCode::to_code;
                                return true;
                            }
                            return false;
                        }
                    }
                }
            }
        } break;
        case 4: {
            if ( s[0] <= 'e' ) {
                if ( s[0] <= 'T' ) {
                    if ( s[0] <= 'N' ) {
                        if ( s[0] <= '!' ) {
                            if ( s == "!<=>" ) {
                                code = ItemCode::not_ltegt_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "None" ) {
                                code = ItemCode::None_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s == "True" ) {
                            code = ItemCode::True_code;
                            return true;
                        }
                        return false;
                    }
                } else {
                    if ( s[0] <= 'c' ) {
                        if ( s == "case" ) {
                            code = ItemCode::case_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( s == "else" ) {
                            code = ItemCode::else_code;
                            return true;
                        }
                        return false;
                    }
                }
            } else {
                if ( s[0] <= 's' ) {
                    if ( s[0] <= 'f' ) {
                        if ( s == "from" ) {
                            code = ItemCode::from_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( s == "skip" ) {
                            code = ItemCode::skip_code;
                            return true;
                        }
                        return false;
                    }
                } else {
                    if ( s == "then" ) {
                        code = ItemCode::then_code;
                        return true;
                    }
                    return false;
                }
            }
        } break;
        case 5: {
            if ( s[0] <= 'e' ) {
                if ( s[0] <= 'c' ) {
                    if ( s[0] <= 'F' ) {
                        if ( s == "False" ) {
                            code = ItemCode::False_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( s == "catch" ) {
                            code = ItemCode::catch_code;
                            return true;
                        }
                        return false;
                    }
                } else {
                    if ( len <= 3 || s[3] <= 'f' ) {
                        if ( len <= 3 || s[3] <= 'd' ) {
                            if ( s == "enddo" ) {
                                code = ItemCode::enddo_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "endfn" ) {
                                code = ItemCode::endfn_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s == "endif" ) {
                            code = ItemCode::endif_code;
                            return true;
                        }
                        return false;
                    }
                }
            } else {
                if ( s[0] <= 'u' ) {
                    if ( s == "until" ) {
                        code = ItemCode::until_code;
                        return true;
                    }
                    return false;
                } else {
                    if ( len <= 2 || s[2] <= 'e' ) {
                        if ( s == "where" ) {
                            code = ItemCode::where_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( s == "while" ) {
                            code = ItemCode::while_code;
                            return true;
                        }
                        return false;
                    }
                }
            }
        } break;
        case 6: {
            if ( len <= 1 || s[1] <= 'l' ) {
                if ( s[0] <= 'd' ) {
                    if ( s[0] <= 'T' ) {
                        if ( s == "Termin" ) {
                            code = ItemCode::Termin_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( len <= 1 || s[1] <= 'e' ) {
                            if ( s == "define" ) {
                                code = ItemCode::define_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "divmod" ) {
                                code = ItemCode::divmod_code;
                                return true;
                            }
                            return false;
                        }
                    }
                } else {
                    if ( s[0] <= 'e' ) {
                        if ( s == "elseif" ) {
                            code = ItemCode::elseif_code;
                            return true;
                        }
                        return false;
                    } else {
                        if ( s == "return" ) {
                            code = ItemCode::return_code;
                            return true;
                        }
                        return false;
                    }
                }
            } else {
                if ( s[0] <= 's' ) {
                    if ( s[0] <= 'e' ) {
                        if ( len <= 3 || s[3] <= 'f' ) {
                            if ( s == "endfor" ) {
                                code = ItemCode::endfor_code;
                                return true;
                            }
                            return false;
                        } else {
                            if ( s == "endtry" ) {
                                code = ItemCode::endtry_code;
                                return true;
                            }
                            return false;
                        }
                    } else {
                        if ( s == "switch" ) {
                            code = ItemCode::switch_code;
                            return true;
                        }
                        return false;
                    }
                } else {
                    if ( s == "unless" ) {
                        code = ItemCode::unless_code;
                        return true;
                    }
                    return false;
                }
            }
        } break;
        case 7: {
            if ( s[0] <= 'd' ) {
                if ( s == "default" ) {
                    code = ItemCode::default_code;
                    return true;
                }
                return false;
            } else {
                if ( s == "finally" ) {
                    code = ItemCode::finally_code;
                    return true;
                }
                return false;
            }
        } break;
        case 10: {
            if ( s == "elseunless" ) {
                code = ItemCode::elseunless_code;
                return true;
            }
            return false;
        } break;
        case 9: {
            if ( len <= 3 || s[3] <= 's' ) {
                if ( len <= 3 || s[3] <= 'd' ) {
                    if ( s == "enddefine" ) {
                        code = ItemCode::enddefine_code;
                        return true;
                    }
                    return false;
                } else {
                    if ( s == "endswitch" ) {
                        code = ItemCode::endswitch_code;
                        return true;
                    }
                    return false;
                }
            } else {
                if ( s == "endunless" ) {
                    code = ItemCode::endunless_code;
                    return true;
                }
                return false;
            }
        } break;
    default: return false;
    }
}

const char * itemCodeToItemKey( ItemCode code )
{
    switch ( code ) {
        case ItemCode::add_code: return "+";
        case ItemCode::and_code: return "and";
        case ItemCode::append_code: return "++";
        case ItemCode::assign_code: return "->";
        case ItemCode::by_code: return "by";
        case ItemCode::case_code: return "case";
        case ItemCode::catch_code: return "catch";
        case ItemCode::cbrace_code: return "}";
        case ItemCode::cbracket_code: return "]";
        case ItemCode::colon_code: return ":";
        case ItemCode::comma_code: return ",";
        case ItemCode::cparen_code: return ")";
        case ItemCode::fat_cparen_code: return "%)";
        case ItemCode::define_code: return "define";
        case ItemCode::Discard_code: return "_";
        case ItemCode::dsemi_code: return ";;";
        case ItemCode::default_code: return "default";
        case ItemCode::div_code: return "div";
        case ItemCode::divmod_code: return "divmod";
        case ItemCode::do_code: return "do";
        case ItemCode::dot_code: return "dot";
        case ItemCode::else_code: return "else";
        case ItemCode::elseif_code: return "elseif";
        case ItemCode::elseunless_code: return "elseunless";
        case ItemCode::end_code: return "end";
        case ItemCode::enddefine_code: return "enddefine";
        case ItemCode::enddo_code: return "enddo";
        case ItemCode::endfn_code: return "endfn";
        case ItemCode::endfor_code: return "endfor";
        case ItemCode::endif_code: return "endif";
        case ItemCode::endswitch_code: return "endswitch";
        case ItemCode::endtry_code: return "endtry";
        case ItemCode::endunless_code: return "endunless";
        case ItemCode::envvar_code: return "$";
        case ItemCode::equal_code: return "=";
        case ItemCode::exact_equal_code: return "==";
        case ItemCode::explode_code: return "...";
        case ItemCode::False_code: return "False";
        case ItemCode::fat_cbrace_code: return "%}";
        case ItemCode::fat_cbracket_code: return "%]";
        case ItemCode::finally_code: return "finally";
        case ItemCode::fn_code: return "fn";
        case ItemCode::for_code: return "for";
        case ItemCode::from_code: return "from";
        case ItemCode::gt_code: return ">";
        case ItemCode::gte_code: return ">=";
        case ItemCode::if_code: return "if";
        case ItemCode::in_code: return "in";
        case ItemCode::lt_code: return "<";
        case ItemCode::lte_code: return "<=";
        case ItemCode::ltgt_code: return "<>";
        case ItemCode::ltegt_code: return "<=>";
        case ItemCode::not_equal_code: return "!=";
        case ItemCode::not_exact_equal_code: return "!==";
        case ItemCode::not_lt_code: return "!<";
        case ItemCode::not_lte_code: return "!<=";
        case ItemCode::not_gt_code: return "!>";
        case ItemCode::not_gte_code: return "!>=";
        case ItemCode::not_ltgt_code: return "!<>";
        case ItemCode::not_ltegt_code: return "!<=>";
        case ItemCode::ltslash_code: return "</";
        case ItemCode::maplet_code: return "=>";
        case ItemCode::mul_code: return "*";
        case ItemCode::not_code: return "not";
        case ItemCode::obrace_code: return "{";
        case ItemCode::obracket_code: return "[";
        case ItemCode::oparen_code: return "(";
        case ItemCode::fat_obrace_code: return "{%";
        case ItemCode::fat_obracket_code: return "[%";
        case ItemCode::fat_oparen_code: return "(%";
        case ItemCode::mod_code: return "mod";
        case ItemCode::None_code: return "None";
        case ItemCode::or_code: return "or";
        case ItemCode::panic_code: return "!!!";
        case ItemCode::pow_code: return "**";
        case ItemCode::return_code: return "return";
        case ItemCode::semi_code: return ";";
        case ItemCode::skip_code: return "skip";
        case ItemCode::slashgt_code: return "/>";
        case ItemCode::sub_code: return "-";
        case ItemCode::switch_code: return "switch";
        case ItemCode::Termin_code: return "Termin";
        case ItemCode::then_code: return "then";
        case ItemCode::True_code: return "True";
        case ItemCode::to_code: return "to";
        case ItemCode::unless_code: return "unless";
        case ItemCode::until_code: return "until";
        case ItemCode::val_code: return "val";
        case ItemCode::var_code: return "var";
        case ItemCode::where_code: return "where";
        case ItemCode::while_code: return "while";
        default: return nullptr;
    }
}

const char * itemCodeToString( ItemCode code )
{
    switch ( code ) {
        case ItemCode::unknown_code: return "unknown_code";
        case ItemCode::int_code: return "int_code";
        case ItemCode::float_code: return "float_code";
        case ItemCode::word_code: return "word_code";
        case ItemCode::string_code: return "string_code";
        case ItemCode::add_code: return "add_code";
        case ItemCode::and_code: return "and_code";
        case ItemCode::append_code: return "append_code";
        case ItemCode::assign_code: return "assign_code";
        case ItemCode::by_code: return "by_code";
        case ItemCode::case_code: return "case_code";
        case ItemCode::catch_code: return "catch_code";
        case ItemCode::cbrace_code: return "cbrace_code";
        case ItemCode::cbracket_code: return "cbracket_code";
        case ItemCode::colon_code: return "colon_code";
        case ItemCode::comma_code: return "comma_code";
        case ItemCode::cparen_code: return "cparen_code";
        case ItemCode::fat_cparen_code: return "fat_cparen_code";
        case ItemCode::define_code: return "define_code";
        case ItemCode::Discard_code: return "Discard_code";
        case ItemCode::dsemi_code: return "dsemi_code";
        case ItemCode::default_code: return "default_code";
        case ItemCode::div_code: return "div_code";
        case ItemCode::divmod_code: return "divmod_code";
        case ItemCode::do_code: return "do_code";
        case ItemCode::dot_code: return "dot_code";
        case ItemCode::else_code: return "else_code";
        case ItemCode::elseif_code: return "elseif_code";
        case ItemCode::elseunless_code: return "elseunless_code";
        case ItemCode::end_code: return "end_code";
        case ItemCode::enddefine_code: return "enddefine_code";
        case ItemCode::enddo_code: return "enddo_code";
        case ItemCode::endfn_code: return "endfn_code";
        case ItemCode::endfor_code: return "endfor_code";
        case ItemCode::endif_code: return "endif_code";
        case ItemCode::endswitch_code: return "endswitch_code";
        case ItemCode::endtry_code: return "endtry_code";
        case ItemCode::endunless_code: return "endunless_code";
        case ItemCode::envvar_code: return "envvar_code";
        case ItemCode::equal_code: return "equal_code";
        case ItemCode::exact_equal_code: return "exact_equal_code";
        case ItemCode::explode_code: return "explode_code";
        case ItemCode::False_code: return "False_code";
        case ItemCode::fat_cbrace_code: return "fat_cbrace_code";
        case ItemCode::fat_cbracket_code: return "fat_cbracket_code";
        case ItemCode::finally_code: return "finally_code";
        case ItemCode::fn_code: return "fn_code";
        case ItemCode::for_code: return "for_code";
        case ItemCode::from_code: return "from_code";
        case ItemCode::gt_code: return "gt_code";
        case ItemCode::gte_code: return "gte_code";
        case ItemCode::if_code: return "if_code";
        case ItemCode::in_code: return "in_code";
        case ItemCode::lt_code: return "lt_code";
        case ItemCode::lte_code: return "lte_code";
        case ItemCode::ltgt_code: return "ltgt_code";
        case ItemCode::ltegt_code: return "ltegt_code";
        case ItemCode::not_equal_code: return "not_equal_code";
        case ItemCode::not_exact_equal_code: return "not_exact_equal_code";
        case ItemCode::not_lt_code: return "not_lt_code";
        case ItemCode::not_lte_code: return "not_lte_code";
        case ItemCode::not_gt_code: return "not_gt_code";
        case ItemCode::not_gte_code: return "not_gte_code";
        case ItemCode::not_ltgt_code: return "not_ltgt_code";
        case ItemCode::not_ltegt_code: return "not_ltegt_code";
        case ItemCode::ltslash_code: return "ltslash_code";
        case ItemCode::maplet_code: return "maplet_code";
        case ItemCode::mul_code: return "mul_code";
        case ItemCode::not_code: return "not_code";
        case ItemCode::obrace_code: return "obrace_code";
        case ItemCode::obracket_code: return "obracket_code";
        case ItemCode::oparen_code: return "oparen_code";
        case ItemCode::fat_obrace_code: return "fat_obrace_code";
        case ItemCode::fat_obracket_code: return "fat_obracket_code";
        case ItemCode::fat_oparen_code: return "fat_oparen_code";
        case ItemCode::mod_code: return "mod_code";
        case ItemCode::None_code: return "None_code";
        case ItemCode::or_code: return "or_code";
        case ItemCode::panic_code: return "panic_code";
        case ItemCode::pow_code: return "pow_code";
        case ItemCode::return_code: return "return_code";
        case ItemCode::semi_code: return "semi_code";
        case ItemCode::skip_code: return "skip_code";
        case ItemCode::slashgt_code: return "slashgt_code";
        case ItemCode::sub_code: return "sub_code";
        case ItemCode::switch_code: return "switch_code";
        case ItemCode::Termin_code: return "Termin_code";
        case ItemCode::then_code: return "then_code";
        case ItemCode::True_code: return "True_code";
        case ItemCode::to_code: return "to_code";
        case ItemCode::unless_code: return "unless_code";
        case ItemCode::until_code: return "until_code";
        case ItemCode::val_code: return "val_code";
        case ItemCode::var_code: return "var_code";
        case ItemCode::where_code: return "where_code";
        case ItemCode::while_code: return "while_code";
        default: return nullptr;
    }
}

ItemCode itemKeyToItemCode( const std::string & key )
{
    ItemCode code;
    lookupItemCode( key, code );
    return code;
}

bool hasItemCode( const std::string & key )
{
    ItemCode code;
    return lookupItemCode( key, code );
}

} // namespace
