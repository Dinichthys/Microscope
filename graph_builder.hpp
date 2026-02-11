#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

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
                                << ((name_.compare("")) ? "TMP" : name_.c_str()) << "  }\"\n"
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
    kAssign
};

const char* const kRed = "red";
const char* const kGreen = "green";

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
                    "[color = \"" << (((type_ == kCopy) || (type_ == kAssign)) ? kRed : kGreen) << "\"];\n\n";
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
            std::ofstream image{kDotFile};
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

            std::string command = std::string("dot -Tsvg ") + kDotFile
                + std::string(" -o Dump_") + std::to_string(image_num_) + std::string(".svg");
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
