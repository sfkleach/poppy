namespace poppy {
    enum class ItemRole { UNKNOWN, PUNCTUATION, PREFIX, VARIABLE, INFIX, POSTFIX, RESERVED };
    std::string itemRoleToString(ItemRole role);
}