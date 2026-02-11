#pragma once

#include <string>

#include "Logger/logging.hpp"
#include "graph_builder.hpp"

#define RED "\033[31m"
#define WHITE "\033[0m"
#define GREEN "\033[32m"

template<class T>
class Micro {
    public:
        static GraphBuilder graph_builder;

    private:
        size_t graph_id_;

        T val_;

        std::string name_;

    public:
        explicit Micro(int val = 0, std::string&& name = "")
            :name_(name) {
            LOG(kDebug, "Constructor was called with arguments:\n"
                        "\tname = \"%s\"\n", name.c_str());
            val_ = val;
            graph_id_ = graph_builder.AddNode(name);
        };

        Micro(Micro& other)
            :name_("copied_" + other.name_) {
            LOG(kDebug, RED "Copy" WHITE " constructor was called with arguments:\n"
                        "\tname = \"%s\"\n", other.name_.c_str());
            val_ = other.val_;
            graph_id_ = graph_builder.AddNode(name_);
            graph_builder.AddEdge(other.graph_id_, graph_id_, kCopy);
        };

        Micro(Micro&& other)
            :name_("moved_" + other.name_) {
            LOG(kDebug, GREEN "Move" WHITE " constructor was called with arguments:\n"
                        "\tname = \"%s\"\n", other.name_.c_str());
            val_ = other.val_;
            graph_id_ = graph_builder.AddNode(name_);
            graph_builder.AddEdge(other.graph_id_, graph_id_, kMove);
        };

#define OP_TWO_ARGS(op, op_name)                                                                \
        Micro operator op(Micro A) {                                                      \
            LOG(kDebug, "%s var \"%s\" and \"%s\"\n", op_name, name_.c_str(), A.name_.c_str()); \
            return Micro(val_ op A.val_, "");                                                \
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
        Micro operator op() {                                                    \
            LOG(kDebug, "%s with var \"%s\" and \"%s\"\n", op_name, name_.c_str()); \
            return Micro(op val_, "");                                           \
        };

        OP_ONE_ARG(+, "Unary plus");
        OP_ONE_ARG(-, "Unary minus");

        OP_ONE_ARG(++, "Prefix increment");
        OP_ONE_ARG(--, "Prefix decrement");

        OP_ONE_ARG(!, "Logical not");

        OP_ONE_ARG(~, "Bin not");

#undef OP_ONE_ARG

        Micro operator ++(int) {
            LOG(kDebug, "Postfix increment var \"%s\"\n", name_.c_str());
            return Micro(val_++, "");
        };
        Micro operator --(int) {
            LOG(kDebug, "Postfix decrement var \"%s\"\n", name_.c_str());
            return Micro(val_--, "");
        };

        Micro& operator =(Micro A) {
            LOG(kDebug, "Assign var \"%s\" and \"%s\"\n", name_.c_str(), A.name_.c_str());
            val_ = A.val_;
            return *this;
        };

        Micro operator[](Micro) = delete;
        Micro operator*() = delete;
        Micro* operator&() {
            LOG(kDebug, "Got pointer on var \"%s\"\n", name_.c_str());
            return this;
        };

        operator T() const {
            return val_;
        };
};

#define MICRO(type, var, val) Micro<type> var(val, #var)
