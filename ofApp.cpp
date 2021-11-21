#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openFrameworks");

	ofBackground(0);
	ofEnableDepthTest();

	auto ico_sphere = ofIcoSpherePrimitive(1, 5);
	this->base_location_list = ico_sphere.getMesh().getVertices();

	this->number_of_sphere = 30;
	while (this->sphere_list.size() < this->number_of_sphere) {

		int index = ofRandom(this->base_location_list.size());
		auto tmp_location = this->base_location_list[index];
		tmp_location = glm::normalize(tmp_location) * ofRandom(0, 110);

		auto radius = this->sphere_list.size() < 30 ? ofRandom(20, 50) : ofRandom(2, 20);

		bool flag = true;
		for (int i = 0; i < this->sphere_list.size(); i++) {

			if (glm::distance(tmp_location, get<0>(this->sphere_list[i])) < get<1>(this->sphere_list[i]) + radius) {

				flag = false;
				break;
			}
		}

		if (flag) {

			auto size = (radius * 2) / sqrt(3);
			this->sphere_list.push_back(make_pair(tmp_location, size));
		}
	}

	ico_sphere = ofIcoSpherePrimitive(300, 3);
	this->triangle_list.insert(this->triangle_list.end(), ico_sphere.getMesh().getUniqueFaces().begin(), ico_sphere.getMesh().getUniqueFaces().end());

	this->frame.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
	this->random_walker.setMode(ofPrimitiveMode::OF_PRIMITIVE_LINES);
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->mesh.clear();
	this->frame.clear();
	this->random_walker.clear();
	this->walker_head.clear();

	for (int i = 0; i < this->sphere_list.size(); i++) {

		auto location = get<0>(this->sphere_list[i]);
		auto radius = get<1>(this->sphere_list[i]) * 0.95;

		for (int k = 0; k < this->triangle_list.size(); k++) {

			glm::vec3 avg = (this->triangle_list[k].getVertex(0) + this->triangle_list[k].getVertex(1) + this->triangle_list[k].getVertex(2)) / 3;
			auto noise_value = ofNoise(glm::vec3(location * 0.1 + avg * 0.005));

			if (noise_value < 0.6) {

				continue;
			}

			vector<glm::vec3> vertices;

			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * (radius + 1) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * (radius + 1) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * (radius + 1) - avg);

			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(0)) * (radius) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(1)) * (radius) - avg);
			vertices.push_back(glm::normalize(this->triangle_list[k].getVertex(2)) * (radius) - avg);

			for (auto& vertex : vertices) {

				vertex += avg + location;
			}

			this->mesh.addVertices(vertices);
			this->frame.addVertices(vertices);

			this->mesh.addColor(ofColor(0));
			this->mesh.addColor(ofColor(0));
			this->mesh.addColor(ofColor(0));

			this->mesh.addColor(ofColor(0));
			this->mesh.addColor(ofColor(0));
			this->mesh.addColor(ofColor(0));

			this->frame.addColor(ofColor(255, 64));
			this->frame.addColor(ofColor(255, 64));
			this->frame.addColor(ofColor(255, 64));

			this->frame.addColor(ofColor(255, 64));
			this->frame.addColor(ofColor(255, 64));
			this->frame.addColor(ofColor(255, 64));

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 4, this->mesh.getNumVertices() - 5, this->mesh.getNumVertices() - 6);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 5);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 5, this->mesh.getNumVertices() - 4);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 3, this->mesh.getNumVertices() - 6);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 1, this->mesh.getNumVertices() - 6, this->mesh.getNumVertices() - 4);

			this->mesh.addTriangle(this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 3, this->mesh.getNumVertices() - 6);
			this->mesh.addTriangle(this->mesh.getNumVertices() - 2, this->mesh.getNumVertices() - 6, this->mesh.getNumVertices() - 5);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 2);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 3);
			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 3);

			this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 5);
			this->frame.addIndex(this->frame.getNumVertices() - 5); this->frame.addIndex(this->frame.getNumVertices() - 6);
			this->frame.addIndex(this->frame.getNumVertices() - 4); this->frame.addIndex(this->frame.getNumVertices() - 6);

			this->frame.addIndex(this->frame.getNumVertices() - 1); this->frame.addIndex(this->frame.getNumVertices() - 4);
			this->frame.addIndex(this->frame.getNumVertices() - 2); this->frame.addIndex(this->frame.getNumVertices() - 5);
			this->frame.addIndex(this->frame.getNumVertices() - 3); this->frame.addIndex(this->frame.getNumVertices() - 6);
		}

		ofColor color;
		for (int k = 0; k < radius * 0.5; k++) {

			int param = int(ofRandom(120) + ofGetFrameNum()) % 120;
			int len = param < 60 ? ofMap(param, 0, 60, 30, 1) : ofMap(param, 60, 120, 1, 30);
			color.setHsb(ofRandom(255), 230, 255);
			auto noise_seed = glm::vec3(ofRandom(1000), ofRandom(1000), ofRandom(1000));

			for (int m = 0; m < 30; m++) {
				
				if (m > len) {

					continue;
				}

				auto vertex = glm::vec3(
					ofMap(ofNoise(noise_seed.x, (ofGetFrameNum() + m) * 0.005), 0, 1, -radius * 0.75, radius * 0.75),
					ofMap(ofNoise(noise_seed.y, (ofGetFrameNum() + m) * 0.005), 0, 1, -radius * 0.75, radius * 0.75),
					ofMap(ofNoise(noise_seed.z, (ofGetFrameNum() + m) * 0.005), 0, 1, -radius * 0.75, radius * 0.75));

				this->random_walker.addVertex(location + vertex);
				this->random_walker.addColor(color);
				if (m > 0) {

					this->random_walker.addIndex(this->random_walker.getNumVertices() - 1);
					this->random_walker.addIndex(this->random_walker.getNumVertices() - 2);
				}

				if (m == 0 | m == len) {

					this->walker_head.addVertex(location + vertex);
					this->walker_head.addColor(color);
				}
			}

		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	this->cam.begin();
	ofRotateY(ofGetFrameNum() * 0.2);

	ofSetLineWidth(2);

	this->mesh.drawFaces();
	this->frame.drawWireframe();
	this->random_walker.drawWireframe();

	ofSetLineWidth(1);

	for (int i = 0; i < this->walker_head.getNumVertices(); i++) {

		ofSetColor(this->walker_head.getColor(i));
		ofDrawSphere(this->walker_head.getVertex(i), 1);
	}

	this->cam.end();
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}