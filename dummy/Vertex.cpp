#include "Vertex.h"

Vertex::Vertex() : Vertex(0)
{
}

Vertex::Vertex(float xy) : Vertex(xy, xy)
{
}

Vertex::Vertex(float x, float y) : x(x), y(y)
{
}
