#pragma once

#include "common.h"
#include "Entity.h"

#include "Material.h"
#include "Buffer.h"

class Billboard : public Entity {

public:

	Billboard(const Material& mat);

	const Material&		getMaterial() const { return material; };
	Material&			getMaterial() { return material; };
	const glm::vec2&	getSize() const { return size; };
	void				setSize(const glm::vec2& size) { this->size = size; };
	float				getSpin() const { return spin; };
	void				setSpin(float spin) { this->spin = spin; };

	virtual void		draw() override;

protected:

	Material material;
	glm::vec2 size;
	float spin;
	std::shared_ptr<Buffer> buffer;


};