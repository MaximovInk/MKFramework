#pragma once
#include "component.h"
#include "texture.h"

#include "../API.h"

namespace MKEngine {
	class MK_ENGINE_API textureComponent : public  MKEngine::component {
	public:
		texture* _texture;

		using component::component;

		virtual void update() {

		};

		virtual void render() {
			if (this->_texture != nullptr) {
				this->_texture->bind();
			}
		};
	};
}