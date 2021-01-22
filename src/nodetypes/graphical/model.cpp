#include "model.hpp"
using namespace sk;

namespace StardustXRServer {

Model::Model() {}

void Model::update() {
	if(queued) {
		queued = false;
		model = sk::model_create_file(modelPath.c_str());
		visible = true;
	}
}

void Model::draw() {
	if(!ready || !visible)
		return;

	if(model != nullptr)
		render_add_model(model, worldTransform());
}

}
