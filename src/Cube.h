#pragma once

#include "common.h"
#include "vertex.h"


struct Cube {
	std::vector<Vertex> vertSide;
	std::vector<uint16_t> indSide;
	std::vector<Vertex> vertTop;
	std::vector<uint16_t> indTop;

};
