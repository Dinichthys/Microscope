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
        inline static GraphBuilder graph_builder;

    private:
        size_t graph_id_;

        T val_;

        std::string name_;

    public:
        explicit Micro(T val = 0, std::string&& name = "")
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

#define OP_TWO_ARGS(op, op_type)                                                                            \
        Micro operator op(Micro A) {                                                                        \
            LOG(kDebug, "%s var \"%s\" and \"%s\"\n", kOpName.at(op_type), name_.c_str(), A.name_.c_str()); \
            Micro res(val_ op A.val_, "");                                                               \
            graph_builder.AddEdge(graph_id_, res.graph_id_, op_type);                                       \
            graph_builder.AddEdge(A.graph_id_, res.graph_id_, op_type);                                     \
            return res;                                                                                     \
        };

        OP_TWO_ARGS(+, kAdd);
        OP_TWO_ARGS(-, kSub);
        OP_TWO_ARGS(*, kMul);
        OP_TWO_ARGS(/, kDiv);
        OP_TWO_ARGS(%, kMod);

        OP_TWO_ARGS(&, kBinAnd);
        OP_TWO_ARGS(|, kBinOr);
        OP_TWO_ARGS(^, kBinXor);
        OP_TWO_ARGS(<<, kBinLeftShift);
        OP_TWO_ARGS(>>, kBinRightShift);

        OP_TWO_ARGS(||, kLogicalOr);
        OP_TWO_ARGS(&&, kLogicalAnd);
        OP_TWO_ARGS(==, kCmpEq);
        OP_TWO_ARGS(!=, kCmpNEq);
        OP_TWO_ARGS(>, kCmpMore);
        OP_TWO_ARGS(<, kCmpLess);
        OP_TWO_ARGS(>=, kCmpMoreEq);
        OP_TWO_ARGS(<=, kCmpLessEq);

        OP_TWO_ARGS(+=, kAddAssign);
        OP_TWO_ARGS(-=, kSubAssign);
        OP_TWO_ARGS(*=, kMulAssign);
        OP_TWO_ARGS(/=, kDivAssign);
        OP_TWO_ARGS(%=, kModAssign);
        OP_TWO_ARGS(&=, kAndAssign);
        OP_TWO_ARGS(|=, kOrAssign);
        OP_TWO_ARGS(^=, kXorAssign);
        OP_TWO_ARGS(<<=, kShiftLeftAssign);
        OP_TWO_ARGS(>>=, kShiftRightAssign);

#undef OP_TWO_ARGS

#define OP_ONE_ARG(op, op_type)                                                                  \
        Micro operator op() {                                                                    \
            LOG(kDebug, "%s with var \"%s\" and \"%s\"\n",  kOpName.at(op_type), name_.c_str()); \
            Micro res(op val_, "");                                                              \
            graph_builder.AddEdge(graph_id_, res.graph_id_, op_type);                                       \
            return res;                                                                          \
        };

        OP_ONE_ARG(+, kUnaryPlus);
        OP_ONE_ARG(-, kUnaryMinus);

        OP_ONE_ARG(++, kPrefixInc);
        OP_ONE_ARG(--, kPrefixDec);

        OP_ONE_ARG(!, kLogicalNot);

        OP_ONE_ARG(~, kBinNot);

#undef OP_ONE_ARG

        Micro operator ++(int) {
            LOG(kDebug, "Postfix increment var \"%s\"\n", name_.c_str());
            Micro res(val_++, "");
            graph_builder.AddEdge(graph_id_, res.graph_id_, kPostfixInc);
            return res;
        };
        Micro operator --(int) {
            LOG(kDebug, "Postfix decrement var \"%s\"\n", name_.c_str());
            Micro res(val_--, "");
            graph_builder.AddEdge(graph_id_, res.graph_id_, kPostfixDec);
            return res;
        };

        Micro& operator =(Micro A) {
            LOG(kDebug, "Assign var \"%s\" and \"%s\"\n", name_.c_str(), A.name_.c_str());
            val_ = A.val_;
            graph_builder.AddEdge(A.graph_id_, graph_id_, kAssign);
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
