#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

class Node {
    private:
        size_t id_;
        std::string name_;

    public:
        Node(size_t id, std::string& name)
            :name_(name) {
            id_ = id;
        };

        void Print(std::ostream& output) {
            output << "\t\"node" << id_ << "\"\n\t[\n"
                                "\t\tlabel = \""
                                "{ " << id_ << " id | "
                                << ((name_.compare("")) ? name_.c_str() : "TMP") << "  }\"\n"
                                "\t\tshape = \"record\"\n"
                                "\t\tstyle = \"filled\"\n"
                                "\t\tfillcolor = \"white\"\n"
                                "\t\tcolor = \"black\"\n"
                                "\t];\n\n";
        }
};

enum Type {
    kCopy,
    kMove,

    kAdd,
    kSub,
    kMul,
    kDiv,
    kMod,

    kBinAnd,
    kBinOr,
    kBinXor,
    kBinLeftShift,
    kBinRightShift,

    kLogicalOr,
    kLogicalAnd,
    kCmpEq,
    kCmpNEq,
    kCmpMore,
    kCmpLess,
    kCmpMoreEq,
    kCmpLessEq,

    kAddAssign,
    kSubAssign,
    kMulAssign,
    kDivAssign,
    kModAssign,
    kAndAssign,
    kOrAssign,
    kXorAssign,
    kShiftLeftAssign,
    kShiftRightAssign,

    kUnaryPlus,
    kUnaryMinus,

    kPrefixInc,
    kPrefixDec,

    kLogicalNot,

    kBinNot,

    kPostfixInc,
    kPostfixDec,

    kAssign
};

const std::map<Type, std::string> kOpName {
    {kCopy, "Copy construct"},
    {kMove, "Move construct"},

    {kAdd, "Add"},
    {kSub, "Sub"},
    {kMul, "Mul"},
    {kDiv, "Div"},
    {kMod, "Mod"},

    {kBinAnd, "Bin and"},
    {kBinOr, "Bin or"},
    {kBinXor, "Bin xor"},
    {kBinLeftShift, "Bin left shift"},
    {kBinRightShift, "Bin right shift"},

    {kLogicalOr, "Logical or"},
    {kLogicalAnd, "Logical and"},
    {kCmpEq, "Comparison (equality)"},
    {kCmpNEq, "Comparison (not equality)"},
    {kCmpMore, "Comparison (more)"},
    {kCmpLess, "Comparison (less)"},
    {kCmpMoreEq, "Comparison (more or equal)"},
    {kCmpLessEq, "Comparison (less or equal)"},

    {kAddAssign, "Add and assign"},
    {kSubAssign, "Sub and assign"},
    {kMulAssign, "Mul and assign"},
    {kDivAssign, "Div and assign"},
    {kModAssign, "Mod and assign"},
    {kAndAssign, "And and assign"},
    {kOrAssign, "Or and assign"},
    {kXorAssign, "Xor and assign"},
    {kShiftLeftAssign, "Shift left and assign"},
    {kShiftRightAssign, "Shift right and assign"},

    {kUnaryPlus, "Unary plus"},
    {kUnaryMinus, "Unary minus"},

    {kPrefixInc, "Prefix increment"},
    {kPrefixDec, "Prefix decrement"},

    {kLogicalNot, "Logical not"},

    {kBinNot, "Bin not"},

    {kPostfixInc, "Postfix increment"},
    {kPostfixDec, "Postfix decrement"},

    {kAssign, "Assign"},
};

const char* const kRed = "red";
const char* const kGreen = "green";
const char* const kBlack = "black";

class Edge {
    private:
        size_t id_start_;
        size_t id_end_;
        Type type_;

    public:
        Edge(size_t id_start, size_t id_end, Type type) {
            id_start_ = id_start;
            id_end_ = id_end;
            type_ = type;
        }

        void Print(std::ostream& output) {
            output << "\t\"node" << id_start_ << "\" -> \"node" << id_end_ << "\""
                    "[color = \"" << ((type_ == kCopy) ? kRed : (type_ == kMove) ? kGreen : kBlack) << "\""
                    " label = \"" << kOpName.at(type_) << "\"];\n\n";
        }
};

const std::string kDumpFolder = "DumpFiles";
const std::string kDotFile = "Dump.dot";

class GraphBuilder {
    private:
        size_t image_num_;

        size_t next_id_;
        std::vector<Node> nodes_;
        std::vector<Edge> edges_;

    public:
        GraphBuilder()
            :image_num_(0), next_id_(0), nodes_(), edges_() {};

        size_t GetNextId() {
            return next_id_++;
        }

        void Draw() {
            std::ofstream image{kDumpFolder + "/" + kDotFile};
            if (!image) {
                std::cerr << "Error creating image!\n";
                return;
            }

            image << "digraph\n{\n"
                    "\tfontname = \"Helvetica,Arial,sans-serif\";\n"
                    "\tnode [fontname = \"Helvetica,Arial,sans-serif\"];\n"
                    "\tgraph [rankdir = \"TB\"];\n"
                    "\tranksep = 1.5;\n"
                    "\tsplines = ortho;\n\n";

            Print(image);

            image << '}';

            image_num_++;

            std::string command = std::string("dot -Tsvg ") + kDumpFolder + "/" + kDotFile
                + " -o" + kDumpFolder + "/Dump_" + std::to_string(image_num_) + std::string(".svg");
            int ret = system(command.c_str());
            if (ret != 0) {
                std::cerr << "Failed to build graph. Return code : " << ret << "\n";
            }
        }

        size_t AddNode(std::string& name) {
            nodes_.push_back(Node(next_id_, name));
            return next_id_++;
        }

        void AddEdge(size_t start, size_t end, Type type) {
            edges_.push_back(Edge(start, end, type));
        }

    private:
        void Print(std::ostream& output) {
            for (auto node : nodes_) {node.Print(output);}
            for (auto edge : edges_) {edge.Print(output);}
        }

};
