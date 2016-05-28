#include "Point.h"

Point::Point() {

}

Point::Point(glm::vec4 coordinates) {
	coords.x = coordinates[0];
	coords.y = coordinates[1];
	coords.z = coordinates[2];
}