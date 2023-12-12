namespace poppy {

    std::string itemRoleToString(ItemRole role) {
        switch (role) {
            case ItemRole::PUNCTUATION:
                return "PUNCTUATION";
            case ItemRole::PREFIX:
                return "PREFIX";
            case ItemRole::VARIABLE:
                return "VARIABLE";
            case ItemRole::INFIX:
                return "INFIX";
            case ItemRole::POSTFIX:
                return "POSTFIX";
            case ItemRole::RESERVED:
                return "RESERVED";
            default:
                return "UNKNOWN";
        }
    }

}
