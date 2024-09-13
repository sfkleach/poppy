#ifndef ITEMATTRS_HPP
#define ITEMATTRS_HPP

#include <string>


namespace poppy {

class ItemKey {
    static const std::string_view KW_ADD;
    static const std::string_view KW_AND;
    static const std::string_view KW_APPEND;
    static const std::string_view KW_ARROW_RIGHT;
    static const std::string_view KW_BY;
    static const std::string_view KW_CASE;
    static const std::string_view KW_CATCH;
    static const std::string_view KW_BRACE_CLOSE;
    static const std::string_view KW_LIST_CLOSE;
    static const std::string_view KW_COLON;
    static const std::string_view KW_COMMA;
    static const std::string_view KW_PAREN_CLOSE;
    static const std::string_view KW_FAT_PAREN_CLOSE;
    static const std::string_view KW_DEFINE;
    static const std::string_view KW_DISCARD;
    static const std::string_view KW_2_SEMICOLON;
    static const std::string_view KW_DEFAULT;
    static const std::string_view KW_DIV;
    static const std::string_view KW_DIV_MOD;
    static const std::string_view KW_DO;
    static const std::string_view KW_INFIX_APPLY_1;
    static const std::string_view KW_ELSE;
    static const std::string_view KW_ELSEIF;
    static const std::string_view KW_ELSEUNLESS;
    static const std::string_view KW_END;
    static const std::string_view KW_ENDDEFINE;
    static const std::string_view KW_ENDDO;
    static const std::string_view KW_ENDLAMBDA;
    static const std::string_view KW_ENDFOR;
    static const std::string_view KW_ENDIF;
    static const std::string_view KW_ENDSWITCH;
    static const std::string_view KW_ENDTRY;
    static const std::string_view KW_ENDUNLESS;
    static const std::string_view KW_DOLLAR;
    static const std::string_view KW_EQUAL;
    static const std::string_view KW_EXACT_EQUAL;
    static const std::string_view KW_EXPLODE;
    static const std::string_view KW_FALSE;
    static const std::string_view KW_FAT_BRACE_CLOSE;
    static const std::string_view KW_FAT_BRACKET_CLOSE;
    static const std::string_view KW_FINALLY;
    static const std::string_view KW_LAMBDA;
    static const std::string_view KW_FOR;
    static const std::string_view KW_FROM;
    static const std::string_view KW_GT;
    static const std::string_view KW_GTE;
    static const std::string_view KW_IF;
    static const std::string_view KW_IN;
    static const std::string_view KW_LT;
    static const std::string_view KW_LTE;
    static const std::string_view KW_LTGT;
    static const std::string_view KW_LTEGT;
    static const std::string_view KW_NOT_EQUAL;
    static const std::string_view KW_NOT_EXACT_EQUAL;
    static const std::string_view KW_NOT_LT;
    static const std::string_view KW_NOT_LTE;
    static const std::string_view KW_NOT_GT;
    static const std::string_view KW_NOT_GTE;
    static const std::string_view KW_NOT_LTGT;
    static const std::string_view KW_NOT_LTEGT;
    static const std::string_view KW_LT_SLASH;
    static const std::string_view KW_MAPLET;
    static const std::string_view KW_MUL;
    static const std::string_view KW_NOT;
    static const std::string_view KW_BRACE_OPEN;
    static const std::string_view KW_LIST_OPEN;
    static const std::string_view KW_PAREN_OPEN;
    static const std::string_view KW_FAT_BRACE_OPEN;
    static const std::string_view KW_FAT_BRACKET_OPEN;
    static const std::string_view KW_FAT_PAREN_OPEN;
    static const std::string_view KW_MOD;
    static const std::string_view KW_NONE;
    static const std::string_view KW_OR;
    static const std::string_view KW_PANIC;
    static const std::string_view KW_POW;
    static const std::string_view KW_RETURN;
    static const std::string_view KW_SEMICOLON;
    static const std::string_view KW_SKIP;
    static const std::string_view KW_SLASH_GT;
    static const std::string_view KW_SUB;
    static const std::string_view KW_SWITCH;
    static const std::string_view KW_TERMIN;
    static const std::string_view KW_THEN;
    static const std::string_view KW_TRUE;
    static const std::string_view KW_TO;
    static const std::string_view KW_UNLESS;
    static const std::string_view KW_UNTIL;
    static const std::string_view KW_VAL;
    static const std::string_view KW_VAR;
    static const std::string_view KW_WHERE;
    static const std::string_view KW_WHILE;
}; // class

enum class ItemCode {
    unknown_code, 
    int_code, 
    float_code, 
    word_code, 
    string_code, 
    add_code, 
    and_code, 
    append_code, 
    assign_code, 
    by_code, 
    case_code, 
    catch_code, 
    cbrace_code, 
    cbracket_code, 
    colon_code, 
    comma_code, 
    cparen_code, 
    fat_cparen_code, 
    define_code, 
    Discard_code, 
    dsemi_code, 
    default_code, 
    div_code, 
    divmod_code, 
    do_code, 
    dot_code, 
    else_code, 
    elseif_code, 
    elseunless_code, 
    end_code, 
    enddefine_code, 
    enddo_code, 
    endfn_code, 
    endfor_code, 
    endif_code, 
    endswitch_code, 
    endtry_code, 
    endunless_code, 
    envvar_code, 
    equal_code, 
    exact_equal_code, 
    explode_code, 
    False_code, 
    fat_cbrace_code, 
    fat_cbracket_code, 
    finally_code, 
    fn_code, 
    for_code, 
    from_code, 
    gt_code, 
    gte_code, 
    if_code, 
    in_code, 
    lt_code, 
    lte_code, 
    ltgt_code, 
    ltegt_code, 
    not_equal_code, 
    not_exact_equal_code, 
    not_lt_code, 
    not_lte_code, 
    not_gt_code, 
    not_gte_code, 
    not_ltgt_code, 
    not_ltegt_code, 
    ltslash_code, 
    maplet_code, 
    mul_code, 
    not_code, 
    obrace_code, 
    obracket_code, 
    oparen_code, 
    fat_obrace_code, 
    fat_obracket_code, 
    fat_oparen_code, 
    mod_code, 
    None_code, 
    or_code, 
    panic_code, 
    pow_code, 
    return_code, 
    semi_code, 
    skip_code, 
    slashgt_code, 
    sub_code, 
    switch_code, 
    Termin_code, 
    then_code, 
    True_code, 
    to_code, 
    unless_code, 
    until_code, 
    val_code, 
    var_code, 
    where_code, 
    while_code, 
}; // enum

bool lookupItemCode( const std::string & s, ItemCode & code );

const char * itemCodeToItemKey( ItemCode code );

const char * itemCodeToString( ItemCode code );

ItemCode itemKeyToItemCode( const std::string & s );

bool hasItemCode( const std::string & s );


} // namespace
#endif
