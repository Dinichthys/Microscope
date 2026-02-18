#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

enum Type {
    kNone,

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

enum NodeType {
    kOp,
    kVar
};

static const std::map<Type, std::string> kOpName {
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

static const char* const kRedEdge = "red";
static const char* const kGreenEdge = "#30A030";
static const char* const kBlackEdge = "black";

static const char* const kGreenNode = "#80FF80";
static const char* const kYellowNode = "#F0FF80";

static const size_t kConstructEdgeWidth = 5;
static const size_t kDefaultEdgeWidth = 1;

class Node {
    private:
        size_t id_;
        std::string name_;
        std::string func_name_;

        NodeType type_;
        Type op_;

    public:
        Node(size_t id, const std::string& name, const std::string func_name)
            :name_(name), func_name_(func_name) {
            id_ = id;
            type_ = kVar;
            op_ = kNone;
        };

        void Print(std::ostream& output) {
            output << "\t\"node" << id_ << "\"\n\t[\n"
                                "\t\tlabel = \""
                                << ((type_ == kOp) ? kOpName.at(op_) :
                                "{ " + std::to_string(id_) + " id | "
                                + ((name_.compare("")) ? name_.c_str() : "TMP") + " }")
                                << "\"\n"
                                << "\t\tshape = \""
                                << ((type_ == kVar) ? "record" : "doubleoctagon")
                                << "\"\n"
                                "\t\tstyle = \"filled\"\n"
                                "\t\tfillcolor = \""
                                << ((name_.compare("")) ? "white" :
                                (((op_ == kCopy) || (op_ == kMove)) ? kYellowNode : kGreenNode))
                                << "\"\n"
                                "\t\tcolor = \"black\"\n"
                                "\n"
                                "\t];\n\n";
        }

        void UpdateName(const std::string& name, const std::string& func_name) {
            name_ = name;
            func_name_ = func_name;
        }

        const char* GetFuncName() const {
            return func_name_.c_str();
        }
        const char* GetName() const {
            return name_.c_str();
        }
        size_t GetID() const {
            return id_;
        }

        void SetType(NodeType type, Type op) {
            type_ = type;
            op_ = op;
        }
};

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
                    "[color = \""
                    << ((type_ == kCopy) ? kRedEdge : (type_ == kMove) ? kGreenEdge : kBlackEdge)
                    << "\""
                    // " label = \""
                    // << kOpName.at(type_)
                    // << "\""
                    << "\t\tpenwidth = "
                    << (((type_ == kCopy) || (type_ == kMove)) ? kConstructEdgeWidth : kDefaultEdgeWidth)
                    << "];\n\n";
        }
};

static const std::string kDumpFolder = "DumpFiles";
static const std::string kDotFile = "Dump.dot";

static const size_t kRGBMax = 255;
static const size_t kStep = 16;
static const size_t kRGBMin = 100;

class GraphBuilder {
    private:
        size_t image_num_;

        std::vector<Node> nodes_;
        std::vector<Edge> edges_;

    public:
        GraphBuilder()
            :image_num_(0), nodes_(), edges_() {};

        void Draw() {
            std::ofstream image{kDumpFolder + "/" + kDotFile};
            if (!image) {
                std::cerr << "Error creating image!\n";
                return;
            }

            image << "digraph\n{\n"
                    "\tcompound=true;\n"
                    "\tfontname = \"Helvetica,Arial,sans-serif\";\n"
                    "\tnode [fontname = \"Helvetica,Arial,sans-serif\"];\n"
                    "\tgraph [rankdir = \"TB\"];\n"
                    "\tranksep = 1.5;\n"
                    "\tsplines = ortho;\n\n";

            Print(image);

            image << '}';

            image_num_++;

            std::string command = std::string("dot -Tsvg ") + kDumpFolder + "/" + kDotFile
                + " -o " + kDumpFolder + "/Dump_" + std::to_string(image_num_) + std::string(".svg");
            int ret = system(command.c_str());
            if (ret != 0) {
                std::cerr << command << std::endl;
                std::cerr << "Failed to build graph. Return code : " << ret << "\n";
            }
        }

        size_t AddNode(const std::string& name, const std::string func_name) {
            nodes_.push_back(Node(nodes_.size(), name, func_name));
            return nodes_.size() - 1;
        }

        void AddEdge(size_t start, size_t end, Type type) {
            if ((type != kCopy) && (type != kMove)) {
                edges_.push_back(Edge(start, end, type));
                nodes_[end].SetType(kOp, type);
            } else {
                size_t res = AddNode("", nodes_[start].GetFuncName());
                nodes_[res].SetType(kOp, type);
                edges_.push_back(Edge(start, res, type));
                edges_.push_back(Edge(res, end, type));
            }
        }

        void UpdateNodeName(size_t id, const std::string& name, const std::string& func_name) {
            nodes_[id].UpdateName(name, func_name);
        }

    private:
        void Print(std::ostream& output) {
            std::map<std::string, std::vector<size_t>> funcs_and_vars = {};

            for (auto node : nodes_) {
                funcs_and_vars[node.GetFuncName()].push_back(node.GetID());
            }

            size_t i = 0;
            for (auto iter = funcs_and_vars.begin(); iter != funcs_and_vars.end(); iter++) {
                auto func = iter;
                output << "\tsubgraph cluster_" << i++ << " {\n"
                    "\t\tnode [style=filled];\n"
                    "\t\tstyle=\"filled\";\n"
                    "\t\tfillcolor=\"#"
                    << std::hex << kRGBMax - kStep
                    << std::hex << kRGBMax - kStep
                    << std::hex << kRGBMax - kStep
                    << "\"\n"
                    "\t\t";

                for (auto var : func->second) {
                    output << " \"node" << var << "\"";
                }

                output << ";\n"
                          "\t\tlabel = \"" << ((func->first.compare("")) ? func->first.c_str() : "TMP") << "\";\n"
                          "\t\tcolor=blue;\n";

                CreateSubGraphs(funcs_and_vars, iter, output, i, 1);

                output << "\t}\n\n";
            }

            for (auto node : nodes_) {
                node.Print(output);
            }

            for (auto edge : edges_) {edge.Print(output);}
        };

        void CreateSubGraphs(const std::map<std::string, std::vector<size_t>>& funcs_and_vars,
                             const std::map<std::string, std::vector<size_t>>::const_iterator& iter,
                             std::ostream& output, size_t& subgraph_num, size_t depth) {
            auto tmp_iter = iter;
            tmp_iter++;
            for (; tmp_iter != funcs_and_vars.end(); tmp_iter++) {
                auto func = tmp_iter;

                if ((iter->first + "/").compare(func->first.substr(0, iter->first.length() + 1))) {
                    continue;
                }

                size_t color = (kRGBMax > kStep *  (depth + 1) + kRGBMin) ? kRGBMax - kStep *  (depth + 1) : kRGBMin;

                output << "\tsubgraph cluster_" << subgraph_num++ << " {\n"
                    "\t\tnode [style=filled];\n"
                    "\t\tstyle=\"filled\";\n"
                    "\t\tfillcolor=\"#"
                    << std::hex << color
                    << std::hex << color
                    << std::hex << color
                    << "\"\n"
                    "\t\t";

                for (auto var : func->second) {
                    output << " \"node" << var << "\"";
                }

                output << ";\n"
                          "\t\tlabel = \"" << ((func->first.compare("")) ? func->first.c_str() : "TMP") << "\";\n"
                          "\t\tcolor=blue;\n";

                CreateSubGraphs(funcs_and_vars, tmp_iter, output, subgraph_num, depth + 1);

                output << "\t}\n\n";
            }

        };

};
