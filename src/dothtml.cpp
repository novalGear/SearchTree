#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>



void generateSVG(const std::string& dotFile, const std::string& svgFile) {
    std::string command = "dot -Tsvg " + dotFile + " -o " + svgFile;
    system(command.c_str());
}

void generateHTML(const std::vector<std::string>& svgFiles, const std::string& htmlFile) {
    std::ofstream html(htmlFile);
    html << "<!DOCTYPE html>\n<html>\n<head>\n";
    html << "<title>Tree Visualization</title>\n";
    html << "<style>img { max-width: 300px; margin: 10px; }</style>\n";
    html << "</head>\n<body>\n";
    html << "<h1>Algorithm Steps</h1>\n";

    for (const auto& svg : svgFiles) {
        html << "<img src=\"" << svg << "\" alt=\"" << svg << "\">\n";
    }

    html << "</body>\n</html>\n";
    html.close();
}
