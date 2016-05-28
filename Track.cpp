#include "Track.h"

// Bernstein matrix
glm::mat4 M = glm::mat4(
	glm::vec4(1.0f, 0.0f, 0.0f, 0.0f),
	glm::vec4(-3.0f, 3.0f, 0.0f, 0.0f),
	glm::vec4(3.0f, -6.0f, 3.0f, 0.0f),
	glm::vec4(-1.0f, 3.0f, -3.0f, 1.0f)
	);

Track::Track(std::vector<std::pair<glm::vec3 *, int>> & selectables) {
	float angle = 0.0f;

	// Define 8 anchor points
	for (int i = 0; angle < 360.0f; ++i, angle += 45.0f) {
		glm::vec3 point = glm::vec3((TRACK_RADIUS + 5.0f) * cos(glm::radians(angle)), (i == 5) ? 3.0f : (i == 2) ? -1.0f : 0.0f, TRACK_RADIUS * sin(glm::radians(angle)));
		printf("ANCHOR: %lf %lf %lf\n", point.x, point.y, point.z);
		this->anchorPoints.push_back(point);
	}

	// Control points between anchor 0 - 1
	this->controlPoints.push_back(glm::vec3(6.0f, 0.0f, -1.5f));
	this->controlPoints.push_back(glm::vec3(10.5f, 0.0f, 3.0f));

	printf("CONTROL: %lf %lf %lf\n", controlPoints[0].x, controlPoints[0].y, controlPoints[0].z);
	printf("CONTROL: %lf %lf %lf\n", controlPoints[1].x, controlPoints[1].y, controlPoints[1].z);

	// Control points between anchor 1 - 2
	this->controlPoints.push_back(glm::vec3(anchorPoints[1] + (anchorPoints[1] - controlPoints[1])));
	this->controlPoints.push_back(glm::vec3(6.0f, 2.0f, 5.5f));

	// Control points between anchor 2 - 3
	this->controlPoints.push_back(glm::vec3(anchorPoints[2] + (anchorPoints[2] - controlPoints[3])));
	this->controlPoints.push_back(glm::vec3(-9.0f, 1.0f, 3.5f));
	
	// Control points between anchor 3 - 4
	this->controlPoints.push_back(glm::vec3(anchorPoints[3] + (anchorPoints[3] - controlPoints[5])));
	this->controlPoints.push_back(glm::vec3(-9.0f, 3.0f, 5.0f));
	
	// Control points between anchor 4 - 5
	this->controlPoints.push_back(glm::vec3(anchorPoints[4] + (anchorPoints[4] - controlPoints[7])));
	this->controlPoints.push_back(glm::vec3(-9.5f, -1.5f, 0.0f));
	
	// Control points between anchor 5 - 6
	this->controlPoints.push_back(glm::vec3(anchorPoints[5] + (anchorPoints[5] - controlPoints[9])));
	this->controlPoints.push_back(glm::vec3(-6.0f, 0.0f, -9.0f));
	
	// Control points between anchor 6 - 7
	this->controlPoints.push_back(glm::vec3(anchorPoints[6] + (anchorPoints[6] - controlPoints[11])));
	this->controlPoints.push_back(glm::vec3(6.0f, 1.0f, -0.75f));

	// Control points between anchor 7 - 0
	this->controlPoints.push_back(glm::vec3(anchorPoints[7] + (anchorPoints[7] - controlPoints[13])));
	this->controlPoints.push_back(glm::vec3(10.5f, 0.0f, -1.0f));
	
	// Recalculate the first control point (from last control point and last anchor)
	this->controlPoints[0] = glm::vec3(anchorPoints[0] + (anchorPoints[0] - controlPoints[15]));

	for (int i = 0; i < NUM_CURVES; ++i) {

		glm::mat4 points;
		
		// Last curve should be drawn between the last anchor point and 
		if (i == NUM_CURVES - 1) {
			points = glm::mat4(glm::vec4(anchorPoints[i], 0.0f),
				glm::vec4(controlPoints[2*i], 0.0f),
				glm::vec4(controlPoints[2*i+1], 0.0f),
				glm::vec4(anchorPoints[0], 0.0f));
		}
		else {
			points = glm::mat4(glm::vec4(anchorPoints[i], 0.0f),
				glm::vec4(controlPoints[2*i], 0.0f),
				glm::vec4(controlPoints[2*i + 1], 0.0f),
				glm::vec4(anchorPoints[i + 1], 0.0f));
		}

		glm::mat4 M_points = points * M;
		std::vector<glm::vec3> bezierPts;

		for (int j = 0; j < 101; ++j) {

			float t = float(j) / 100.0f;
			glm::vec3 result = glm::vec3(M_points * glm::vec4(1.0f, t, t*t, t*t*t));
			// Set max value for current track point
			if (result.y > maxTrackHeight.y) {
				maxTrackHeight = result;
				currTime = t;
				currCurve = i;
				savedHighestTime = t;
				savedHighestCurve = i;
			}

			bezierPts.push_back(result);
		}

		curves.push_back(new Bezier(bezierPts));
	}

	currTrackPoint = maxTrackHeight;

	// Shift control points over
	glm::vec3 saved = controlPoints[0];
	for (int i = 0; i < controlPoints.size()-1; ++i) {
		controlPoints[i] = controlPoints[i + 1];
	}

	controlPoints[controlPoints.size() - 1] = saved;

	
	for (int i = 0; i < anchorPoints.size(); ++i) {
		selectables.push_back(std::make_pair(&anchorPoints[i], i));
	}

	for (int i = 0; i < controlPoints.size(); ++i) {
		selectables.push_back(std::make_pair(&controlPoints[i], -1));
	}
	

	//Generate buffers for anchor points
	glGenVertexArrays(1, &anchorVAO);
	glGenBuffers(1, &anchorVBO);

	glBindVertexArray(anchorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, anchorVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * anchorPoints.size(), &(anchorPoints[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	
	//Generate buffers for control points
	glGenVertexArrays(1, &controlVAO);
	glGenBuffers(1, &controlVBO);

	glBindVertexArray(controlVAO);
	glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * controlPoints.size(), &(controlPoints[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Track::draw() {
	// Draw the main track curve
	for (int i = 0; i < curves.size(); ++i) {
		curves[i]->draw();
	}
	
	// Re-bind the VAO
	glm::mat4 MVP = Window::P * Window::V * this->toWorld;
	GLuint MatrixID = glGetUniformLocation(Window::trackShader, "MVP");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
	
	glUniform1i(glGetUniformLocation(Window::trackShader, "isAnchor"), 1);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControl"), 0);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControlLine"), 0);

	// Draw the anchor points
	glBindVertexArray(anchorVAO);
	glPointSize(9.0f);
	glDrawArrays(GL_POINTS, 0, anchorPoints.size());
	glBindVertexArray(0);
	
	glUniform1i(glGetUniformLocation(Window::trackShader, "isAnchor"), 0);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControl"), 1);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControlLine"), 0);

	// Draw the control points
	glBindVertexArray(controlVAO);
	glPointSize(9.0f);
	glDrawArrays(GL_POINTS, 0, controlPoints.size());
	glBindVertexArray(0);

	glUniform1i(glGetUniformLocation(Window::trackShader, "isControl"), 0);
	glUniform1i(glGetUniformLocation(Window::trackShader, "isControlLine"), 1);

	// Draw a line through the control points
	glBindVertexArray(controlVAO);
	glPointSize(3.0f);
	glDrawArrays(GL_LINES, 0, controlPoints.size());
	glBindVertexArray(0);
	
}

void Track::redraw()
{
	reCalculatePoints();

	// Draw the main track curve
	glBindVertexArray(anchorVAO);
	glBindBuffer(GL_ARRAY_BUFFER, anchorVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * anchorPoints.size(), &(anchorPoints[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	//Generate buffers for control points
	glGenVertexArrays(1, &controlVAO);
	glGenBuffers(1, &controlVBO);

	glBindVertexArray(controlVAO);
	glBindBuffer(GL_ARRAY_BUFFER, controlVBO);
	glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(GLfloat) * controlPoints.size(), &(controlPoints[0]), GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,
		3,
		GL_FLOAT,
		GL_FALSE,
		3 * sizeof(GLfloat),
		(GLvoid*)0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Track::reCalculatePoints()
{
	for (int i = 0; i < NUM_CURVES; ++i) {

		glm::mat4 points;

		// Last curve should be drawn between the last anchor point and last control points
		if (i == NUM_CURVES - 1) {
			points = glm::mat4(glm::vec4(anchorPoints[i], 0.0f),
			glm::vec4(controlPoints[2 * i], 0.0f),
			glm::vec4(controlPoints[2 * i + 1], 0.0f),
			glm::vec4(anchorPoints[0], 0.0f));
		}
		else {
			points = glm::mat4(glm::vec4(anchorPoints[i], 0.0f),
			glm::vec4(controlPoints[2 * i], 0.0f),
			glm::vec4(controlPoints[2 * i + 1], 0.0f),
			glm::vec4(anchorPoints[i + 1], 0.0f));
		}
		glm::mat4 M_points = points * M;

		std::vector<glm::vec3> bezierPts;

		for (int j = 0; j < 101; ++j) {

			float t = float(j) / 100.0f;
			glm::vec3 result = glm::vec3(M_points * glm::vec4(1.0f, t, t*t, t*t*t));

			// Mayhaps we have a new highest point
			if (result.y > maxTrackHeight.y) {
				maxTrackHeight = result;
				savedHighestTime = t;
				savedHighestCurve = i;
			}

			bezierPts.push_back(result);
		}

		curves[i] = new Bezier(bezierPts);
	}
}

void Track::translateSelected(glm::vec3 * selected, glm::vec3 xAmt, glm::vec3 yAmt)
{
	*selected = *selected + xAmt + yAmt;

	for (int i = 0; i < anchorPoints.size(); ++i) {
		// Anchor point!
		if (selected == &anchorPoints[i]) {

			if (i == 0) {
				// move the control points also
				controlPoints[15] += xAmt + yAmt;
				controlPoints[14] += xAmt + yAmt;
			}
			else {
				controlPoints[2 * i - 1] += xAmt + yAmt;
				controlPoints[2 * i - 2] += xAmt + yAmt;
			}
			return;
		}
	}

	for (int i = 0; i < controlPoints.size(); ++i) {
		// Control point!
		if (selected == &controlPoints[i]) {

			// Corresponding control point is after
			if (i % 2 == 0) {
				controlPoints[i + 1] -= xAmt + yAmt;
			}
			// Corresponding control is before
			else 
				controlPoints[i - 1] -= xAmt + yAmt;
			return;
		}
	}
	
}

glm::vec3 Track::getHighestPoint()
{
	return this->maxTrackHeight;
}

glm::vec3 Track::getNewPos()
{
	int timeIndex = (int)(currTime * 100.0f);
	float currHeight = curves[currCurve]->points[timeIndex].y;
	float nextHeight;

	// Next height might be on next curve
	if (timeIndex == 100) {
		++currCurve;
		nextHeight = curves[currCurve]->points[0].y;
	}
	else {
		nextHeight = curves[currCurve]->points[timeIndex + 1].y;
	}

	// Find difference between both heights
	float deltaHeight = maxTrackHeight.y - currHeight;
	float velocity;

	// Going uphill (backwards)
	if (deltaHeight < 0.0f) {
		printf("Counterclockwise\n");
		velocity = -sqrt(0.001f * -deltaHeight);
		currTime += velocity;
	}
	// Going downhill (forwards/clockwise)
	else {
		printf("Clockwise\n");
		velocity = sqrt(0.001f * deltaHeight);
		currTime += velocity;
	}

	if (velocity == 0.0f) {
		velocity = 0.01f;
		currTime += velocity;
	}

	// if currTime is out of bounds, subtract/add until it is
	while (currTime < 0.0f) {
		currTime += 1.0f;
		currCurve = (currCurve == 0) ? 7 : currCurve - 1;
	}

	while (currTime > 1.0f) {
		currTime -= 1.0f;
		currCurve = (currCurve == 7) ? 0 : currCurve + 1;
	}

	// Determine where on the curve to go next
	currTrackPoint = curves[currCurve]->points[(int)(currTime * 100.0f)];
	currTrackPoint.y += 0.3f;
	return currTrackPoint;
}

void Track::resetMovement()
{
	// Reset t and currTrackPoint
	currTrackPoint = maxTrackHeight;
	currTime = savedHighestTime;
	currCurve = savedHighestCurve;
}