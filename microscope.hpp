#pragma once

#include <string>

#include "Logger/logging.hpp"

#define RED "\033[31m"
#define WHITE "\033[0m"
#define GREEN "\033[32m"


class MicroInt {
    private:
        int val_;

        std::string name_;

    public:
        explicit MicroInt(int val = 0, std::string&& name = "")
            :name_(name) {
            LOG(kDebug, "Constructor was called with arguments:\n"
                        "\tval = \"%d\"\n"
                        "\tname = \"%s\"\n", val, name.c_str());
            val_ = val;
        };

        MicroInt(MicroInt& other)
            :name_("copied_" + other.name_) {
            LOG(kDebug, RED "Copy" WHITE " constructor was called with arguments:\n"
                        "\tval = \"%d\"\n"
                        "\tname = \"%s\"\n", other.val_, other.name_.c_str());
            val_ = other.val_;
        };

        MicroInt(MicroInt&& other)
            :name_("moved_" + other.name_) {
            LOG(kDebug, GREEN "Move" WHITE " constructor was called with arguments:\n"
                        "\tval = \"%d\"\n"
                        "\tname = \"%s\"\n", other.val_, other.name_.c_str());
            val_ = other.val_;
        };

#define OP_TWO_ARGS(op, op_name)                                                                \
        MicroInt operator op(MicroInt A) {                                                      \
            LOG(kDebug, "%s var \"%s\" and \"%s\"\n", op_name, name_.c_str(), A.name_.c_str()); \
            return MicroInt(val_ op A.val_, "");                                                \
        };

        OP_TWO_ARGS(+, "Add");
        OP_TWO_ARGS(-, "Sub");
        OP_TWO_ARGS(*, "Mul");
        OP_TWO_ARGS(/, "Div");
        OP_TWO_ARGS(%, "Mod");

        OP_TWO_ARGS(&, "Bin and");
        OP_TWO_ARGS(|, "Bin or");
        OP_TWO_ARGS(^, "Bin xor");
        OP_TWO_ARGS(<<, "Bin left shift");
        OP_TWO_ARGS(>>, "Bin right shift");

        OP_TWO_ARGS(||, "Logical or");
        OP_TWO_ARGS(&&, "Logical and");
        OP_TWO_ARGS(==, "Comparison (equality)");
        OP_TWO_ARGS(!=, "Comparison (not equality)");
        OP_TWO_ARGS(>, "Comparison (more)");
        OP_TWO_ARGS(<, "Comparison (less)");
        OP_TWO_ARGS(>=, "Comparison (more or equal)");
        OP_TWO_ARGS(<=, "Comparison (less or equal)");

        OP_TWO_ARGS(+=, "Add and assign");
        OP_TWO_ARGS(-=, "Sub and assign");
        OP_TWO_ARGS(*=, "Mul and assign");
        OP_TWO_ARGS(/=, "Div and assign");
        OP_TWO_ARGS(%=, "Mod and assign");
        OP_TWO_ARGS(&=, "And and assign");
        OP_TWO_ARGS(|=, "Or and assign");
        OP_TWO_ARGS(^=, "Xor and assign");
        OP_TWO_ARGS(<<=, "Shift left and assign");
        OP_TWO_ARGS(>>=, "Shift right and assign");

#undef OP_TWO_ARGS

#define OP_ONE_ARG(op, op_name)                                                     \
        MicroInt operator op() {                                                    \
            LOG(kDebug, "%s with var \"%s\" and \"%s\"\n", op_name, name_.c_str()); \
            return MicroInt(op val_, "");                                           \
        };

        OP_ONE_ARG(+, "Unary plus");
        OP_ONE_ARG(-, "Unary minus");

        OP_ONE_ARG(++, "Prefix increment");
        OP_ONE_ARG(--, "Prefix decrement");

        OP_ONE_ARG(!, "Logical not");

        OP_ONE_ARG(~, "Bin not");

#undef OP_ONE_ARG

        MicroInt operator ++(int) {
            LOG(kDebug, "Postfix increment var \"%s\"\n", name_.c_str());
            return MicroInt(val_++, "");
        };
        MicroInt operator --(int) {
            LOG(kDebug, "Postfix decrement var \"%s\"\n", name_.c_str());
            return MicroInt(val_--, "");
        };

        MicroInt& operator =(MicroInt A) {
            LOG(kDebug, "Assign var \"%s\" and \"%s\"\n", name_.c_str(), A.name_.c_str());
            val_ = A.val_;
            return *this;
        };

        MicroInt operator[](MicroInt) = delete;
        MicroInt operator*() = delete;
        MicroInt* operator&() {
            LOG(kDebug, "Got pointer on var \"%s\"\n", name_.c_str());
            return this;
        };

        operator int() const {
            return val_;
        };

// #define BOOL_OP_TWO_ARGS(op, op_name)                                                       \
         bool operator op(MicroInt A) {                                                      \
             LOG(kDebug, "%s var \"%s\" and \"%s\"\n", op_name, name_.c_str(), A.name_.c_str()); \
             return val_ op A.val_;                                                          \
         };
//
//         BOOL_OP_TWO_ARGS(||, "Logical or");
//         BOOL_OP_TWO_ARGS(&&, "Logical and");
//         BOOL_OP_TWO_ARGS(==, "Comparison (equality)");
//         BOOL_OP_TWO_ARGS(!=, "Comparison (not equality)");
//         BOOL_OP_TWO_ARGS(>, "Comparison (more)");
//         BOOL_OP_TWO_ARGS(<, "Comparison (less)");
//         BOOL_OP_TWO_ARGS(>=, "Comparison (more or equal)");
//         BOOL_OP_TWO_ARGS(<=, "Comparison (less or equal)");
//
// #undef BOOL_OP_TWO_ARGS

};

#define MICRO_INT(var, val) MicroInt var(val, #var)
