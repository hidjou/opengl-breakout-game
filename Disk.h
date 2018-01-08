#ifndef DISK_H
#define DISK_H

#include "vertex.h"

#define PI 3.14159265
#define DISK_SEGS 100 // Number of segments.
#define DISK_AREA 130 // Number of segments.

//#define DISK_COLORS 1.0, 1.0, 0.0, 1.0 // colour

void fillDiscVertexArray(Vertex discVertices[DISK_SEGS]);

#endif
