#ifndef TRACK_H
#define TRACK_H

#include "Track.h"
#include "Bezier.h"
#include "Window.h"

class Bezier;

class Track {
private:
	int NUM_CURVES = 8;
	float TRACK_RADIUS = 7.0f;
	GLuint anchorVAO, anchorVBO;
	GLuint controlVAO, controlVBO;
	glm::mat4 toWorld;
public:
	Track(std::vector<std::pair<glm::vec3 *, int>> &);
	std::vector<Bezier *> curves;
	std::vector<glm::vec3> anchorPoints;
	std::vector<glm::vec3> controlPoints;

	glm::vec3 currTrackPoint;
	float currTime;
	float savedHighestTime;
	int savedHighestCurve;
	int currCurve;
	glm::vec3 maxTrackHeight;

	glm::vec3 getNewPos();
	void draw();
	void redraw();
	void reCalculatePoints();
	void translateSelected(glm::vec3 * selected, glm::vec3, glm::vec3);
	glm::vec3 getHighestPoint();
	void resetMovement();
};

#endif