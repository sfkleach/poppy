#include <string>

#include "itemrole.hpp"
#include "itemattrs.hpp"


namespace poppy {

    std::string itemRoleToString(ItemRole role) {
        switch (role) {
            case ItemRole::PUNCTUATION:
                return "PUNCTUATION";
            case ItemRole::PREFIX:
                return "PREFIX";
            case ItemRole::VARIABLE:
                return "VARIABLE";
            case ItemRole::CONSTANT:
                return "CONSTANT";
            case ItemRole::INFIX:
                return "INFIX";
            case ItemRole::POSTFIX:
                return "POSTFIX";
            case ItemRole::BOTHFIX:
                return "BOTHFIX";
            case ItemRole::RESERVED:
                return "RESERVED";
            default:
                return "UNKNOWN";
        }
    }

    ItemRole itemCodeToItemRole(ItemCode code) {
        switch (code) {
            case ItemCode::unknown_code: return ItemRole::UNKNOWN;
            case ItemCode::int_code: return ItemRole::CONSTANT;
            case ItemCode::float_code: return ItemRole::CONSTANT;
            case ItemCode::word_code: return ItemRole::VARIABLE;
            case ItemCode::string_code: return ItemRole::CONSTANT;
            case ItemCode::add_code: return ItemRole::INFIX;
            case ItemCode::and_code: return ItemRole::UNKNOWN;
            case ItemCode::append_code: return ItemRole::UNKNOWN;
            case ItemCode::assign_code: return ItemRole::UNKNOWN;
            case ItemCode::by_code: return ItemRole::UNKNOWN;
            case ItemCode::case_code: return ItemRole::PUNCTUATION;
            case ItemCode::catch_code: return ItemRole::RESERVED;
            case ItemCode::cbrace_code: return ItemRole::RESERVED;
            case ItemCode::cbracket_code: return ItemRole::RESERVED;
            case ItemCode::colon_code: return ItemRole::PUNCTUATION;
            case ItemCode::comma_code: return ItemRole::PUNCTUATION;
            case ItemCode::cparen_code: return ItemRole::PUNCTUATION;
            case ItemCode::fat_cparen_code: return ItemRole::PUNCTUATION;
            case ItemCode::define_code: return ItemRole::PREFIX;
            case ItemCode::Discard_code: return ItemRole::VARIABLE;
            case ItemCode::dsemi_code: return ItemRole::UNKNOWN;
            case ItemCode::default_code: return ItemRole::UNKNOWN;
            case ItemCode::div_code: return ItemRole::UNKNOWN;
            case ItemCode::divmod_code: return ItemRole::UNKNOWN;
            case ItemCode::do_code: return ItemRole::UNKNOWN;
            case ItemCode::dot_code: return ItemRole::UNKNOWN;
            case ItemCode::else_code: return ItemRole::PUNCTUATION;
            case ItemCode::elseif_code: return ItemRole::PUNCTUATION;
            case ItemCode::elseunless_code: return ItemRole::RESERVED;
            case ItemCode::end_code: return ItemRole::PUNCTUATION;
            case ItemCode::enddefine_code: return ItemRole::PUNCTUATION;
            case ItemCode::enddo_code: return ItemRole::UNKNOWN;
            case ItemCode::endfn_code: return ItemRole::PUNCTUATION;
            case ItemCode::endfor_code: return ItemRole::PUNCTUATION;
            case ItemCode::endif_code: return ItemRole::PUNCTUATION;
            case ItemCode::endswitch_code: return ItemRole::PUNCTUATION;
            case ItemCode::endtry_code: return ItemRole::RESERVED;
            case ItemCode::endunless_code: return ItemRole::PUNCTUATION;
            case ItemCode::envvar_code: return ItemRole::UNKNOWN;
            case ItemCode::equal_code: return ItemRole::UNKNOWN;
            case ItemCode::exact_equal_code: return ItemRole::UNKNOWN;
            case ItemCode::explode_code: return ItemRole::UNKNOWN;
            case ItemCode::False_code: return ItemRole::CONSTANT;
            case ItemCode::fat_cbrace_code: return ItemRole::UNKNOWN;
            case ItemCode::fat_cbracket_code: return ItemRole::UNKNOWN;
            case ItemCode::finally_code: return ItemRole::UNKNOWN;
            case ItemCode::fn_code: return ItemRole::UNKNOWN;
            case ItemCode::for_code: return ItemRole::PREFIX;
            case ItemCode::from_code: return ItemRole::UNKNOWN;
            case ItemCode::gt_code: return ItemRole::UNKNOWN;
            case ItemCode::gte_code: return ItemRole::UNKNOWN;
            case ItemCode::if_code: return ItemRole::PREFIX;
            case ItemCode::in_code: return ItemRole::PUNCTUATION;
            case ItemCode::lt_code: return ItemRole::UNKNOWN;
            case ItemCode::lte_code: return ItemRole::UNKNOWN;
            case ItemCode::ltgt_code: return ItemRole::UNKNOWN;
            case ItemCode::ltegt_code: return ItemRole::UNKNOWN;
            case ItemCode::not_equal_code: return ItemRole::UNKNOWN;
            case ItemCode::not_exact_equal_code: return ItemRole::UNKNOWN;
            case ItemCode::not_lt_code: return ItemRole::UNKNOWN;
            case ItemCode::not_lte_code: return ItemRole::UNKNOWN;
            case ItemCode::not_gt_code: return ItemRole::UNKNOWN;
            case ItemCode::not_gte_code: return ItemRole::UNKNOWN;
            case ItemCode::not_ltgt_code: return ItemRole::UNKNOWN;
            case ItemCode::not_ltegt_code: return ItemRole::UNKNOWN;
            case ItemCode::ltslash_code: return ItemRole::UNKNOWN;
            case ItemCode::maplet_code: return ItemRole::UNKNOWN;
            case ItemCode::mul_code: return ItemRole::UNKNOWN;
            case ItemCode::not_code: return ItemRole::UNKNOWN;
            case ItemCode::obrace_code: return ItemRole::UNKNOWN;
            case ItemCode::obracket_code: return ItemRole::UNKNOWN;
            case ItemCode::oparen_code: return ItemRole::PUNCTUATION;
            case ItemCode::fat_obrace_code: return ItemRole::RESERVED;
            case ItemCode::fat_obracket_code: return ItemRole::RESERVED;
            case ItemCode::fat_oparen_code: return ItemRole::RESERVED;
            case ItemCode::mod_code: return ItemRole::UNKNOWN;
            case ItemCode::None_code: return ItemRole::UNKNOWN;
            case ItemCode::or_code: return ItemRole::UNKNOWN;
            case ItemCode::panic_code: return ItemRole::UNKNOWN;
            case ItemCode::pow_code: return ItemRole::UNKNOWN;
            case ItemCode::return_code: return ItemRole::UNKNOWN;
            case ItemCode::semi_code: return ItemRole::UNKNOWN;
            case ItemCode::skip_code: return ItemRole::UNKNOWN;
            case ItemCode::slashgt_code: return ItemRole::UNKNOWN;
            case ItemCode::sub_code: return ItemRole::UNKNOWN;
            case ItemCode::switch_code: return ItemRole::UNKNOWN;
            case ItemCode::Termin_code: return ItemRole::RESERVED;
            case ItemCode::then_code: return ItemRole::PUNCTUATION;
            case ItemCode::True_code: return ItemRole::CONSTANT;
            case ItemCode::to_code: return ItemRole::UNKNOWN;
            case ItemCode::unless_code: return ItemRole::RESERVED;
            case ItemCode::until_code: return ItemRole::RESERVED;
            case ItemCode::val_code: return ItemRole::UNKNOWN;
            case ItemCode::var_code: return ItemRole::UNKNOWN;
            case ItemCode::where_code: return ItemRole::UNKNOWN;
            case ItemCode::while_code:return ItemRole::UNKNOWN;
            default: return ItemRole::UNKNOWN;
        }
    }

}