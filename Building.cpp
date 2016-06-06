#include "Building.h"

Building::Building(int numComponents, int width, glm::vec3 origin, int type)
{
	float height;
	float savedHeight = 0.0f;
	float widthF = width;

	switch(type) {
		case TOWER: {
			for (int i = 0; i < numComponents; ++i) {
				//int type = rand() % 3 + 1;
				// Defines a max height
				height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
				height += rand() % 2 + (-1.0f);
				int type = (i == numComponents - 1) ? BuildingComponent::PYRAMID : BuildingComponent::RECTANGLE;
				if (i == 0) {
					components.push_back(new BuildingComponent(glm::vec3(widthF, height, widthF), glm::vec3(origin.x, i, 0.0f), type));
				}
				else {
					float widthFactor = (float)rand() / RAND_MAX;
					widthFactor = (widthFactor < 0.5f) ? 0.65f : widthFactor;
					components.push_back(new BuildingComponent(glm::vec3(widthF * widthFactor, height, widthF * widthFactor), glm::vec3(origin.x, savedHeight, 0.0f), type));
				}
				savedHeight += height;
			}
			break;
		}
		case ARCH: {
			float pillarWidth = widthF / 3.0f;
			// Build both pillars
			height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
			height += rand() % 2 + (-1.0f);
			components.push_back(new BuildingComponent(glm::vec3(pillarWidth, height, widthF), glm::vec3(origin.x - pillarWidth, 0.0f, 0.0f), BuildingComponent::RECTANGLE));
			components.push_back(new BuildingComponent(glm::vec3(pillarWidth, height, widthF), glm::vec3(origin.x + pillarWidth, 0.0f, 0.0f), BuildingComponent::RECTANGLE));

			savedHeight += height;
			height = ((float)rand() / RAND_MAX);
			// Then rectangle on top
			components.push_back(new BuildingComponent(glm::vec3(widthF,height, widthF), glm::vec3(origin.x, savedHeight, 0.0f), BuildingComponent::RECTANGLE));
			savedHeight += height;
			height = ((float)rand() / RAND_MAX);
			height = (height < 0.5f) ? 0.5f : height;
			// Then trapezoid
			components.push_back(new BuildingComponent(glm::vec3(widthF, height, widthF), glm::vec3(origin.x, savedHeight, 0.0f), BuildingComponent::ARCH));

			break;
		}
		case SKYSCRAPER: {
			float currWidth = widthF;
			for (int i = 0; i < numComponents; ++i) {
				//int type = rand() % 3 + 1;
				// Defines a max height
				height = ((float)rand() / RAND_MAX) * 3.0f + 1.5f;
				height += rand() % 2 + (-1.0f);
				if (i == 0) {
					components.push_back(new BuildingComponent(glm::vec3(currWidth, height, currWidth), glm::vec3(origin.x, i, 0.0f), BuildingComponent::RECTANGLE));
				}
				else {
					float widthFactor = (float)rand() / RAND_MAX;
					widthFactor = (widthFactor < 0.5f) ? 0.65f : widthFactor;
					components.push_back(new BuildingComponent(glm::vec3(currWidth * widthFactor, height, currWidth * widthFactor), glm::vec3(origin.x, savedHeight, 0.0f), type));
					currWidth *= widthFactor;
				}
				savedHeight += height;
			}

			break;
		}
		default:
			break;
	}

	buildingTexture = loadTexture();
}

void Building::draw(GLint shaderProgram)
{
	for (int i = 0; i < components.size(); ++i) {
		glUniform1i(glGetUniformLocation(shaderProgram, "buildingPart"), components[i]->componentType);
		components[i]->bindTexture(buildingTexture);
		components[i]->draw(shaderProgram);
	}
}

unsigned char* Building::loadPPM(const char* filename, int& width, int& height)
{
	const int BUFSIZE = 128;
	FILE* fp;
	unsigned int read;
	unsigned char* rawData;
	char buf[3][BUFSIZE];
	char* retval_fgets;
	size_t retval_sscanf;

	if ((fp = fopen(filename, "rb")) == NULL)
	{
		std::cerr << "error reading ppm file, could not locate " << filename << std::endl;
		width = 0;
		height = 0;
		return NULL;
	}

	// Read magic number:
	retval_fgets = fgets(buf[0], BUFSIZE, fp);

	// Read width and height:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');
	retval_sscanf = sscanf(buf[0], "%s %s", buf[1], buf[2]);
	width = atoi(buf[1]);
	height = atoi(buf[2]);

	// Read maxval:
	do
	{
		retval_fgets = fgets(buf[0], BUFSIZE, fp);
	} while (buf[0][0] == '#');

	// Read image data:
	rawData = new unsigned char[width * height * 3];
	read = fread(rawData, width * height * 3, 1, fp);
	fclose(fp);

	if (read != 1)
	{
		std::cerr << "error parsing ppm file, incomplete data" << std::endl;
		delete[] rawData;
		width = 0;
		height = 0;
		return NULL;
	}

	return rawData;
}

GLuint Building::loadTexture()
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_2D, textureID);
	image = loadPPM("city1.ppm", width, height);

	if (image == NULL) {
		printf("Texture was null!\n");
		delete(image);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_COMPRESSED_RGB, 
		width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);

	delete(image);

	printf("Building texture: %d\n", textureID);

	return textureID;
}