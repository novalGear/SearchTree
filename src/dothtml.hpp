#pragma once

#include <string>
#include <vector>

void generateSVG(const std::string& dotFile, const std::string& svgFile);
void generateHTML(const std::vector<std::string>& svgFiles, const std::string& htmlFile);
