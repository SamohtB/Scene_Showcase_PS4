#include "AGameObject.h"

AGameObject::AGameObject(String name) : name(name)
{
}

AGameObject::~AGameObject()
{
}

std::string AGameObject::getName()
{
	return this->name;
}

void AGameObject::setName(String name)
{
	this->name = name;
}
