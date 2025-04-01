#include "AGameObject.h"

AGameObject::AGameObject(String name) : name(name)
{
}

AGameObject::~AGameObject()
{
}

AGameObject::String AGameObject::getName()
{
	return this->name;
}

void AGameObject::setName(String name)
{
	this->name = name;
}

AGameObject::TexturePtr AGameObject::getTexture()
{
	return this->texture;
}

void AGameObject::setTexture(TexturePtr texture)
{
	this->texture = texture;
}

AGameObject::MeshPtr AGameObject::getMesh()
{
	return this->mesh;
}

void AGameObject::setMesh(MeshPtr mesh)
{
	this->mesh = mesh;
}
