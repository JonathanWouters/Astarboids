#pragma once

//STL includes
#include <wchar.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <algorithm>
#include <iomanip>

//OPENGL includes
#include <SDL.h>
#include <GL/glew.h>

//GLM
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtx/vector_angle.hpp>

//Extra math
#include "Engine\Math\Math.h"

//DEBUG
#include "Engine\Diagnostics\Logger.h"

//MACROS
#define UNREFERENCED_PARAMETER(P) (P);
#define ZeroMemory(Destination,Length) memset((Destination),0,(Length))