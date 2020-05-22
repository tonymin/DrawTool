#pragma once

namespace Core {
namespace Graphics {

// definitions for canvas
const int KCanvasGridSpace = 20;
const int KCanvasViewX = 0;
const int KCanvasViewY = 0;
const int KCanvasViewDefaultWidth = 1000;
const int KCanvasViewDefaultHeight = 1000;
const double KZoomScale = 1.25;

enum class EDrawMode
{
	None,
	Line,
	SpLine
};


}
}
