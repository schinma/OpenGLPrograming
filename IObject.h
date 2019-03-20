#pragma once

#ifndef I_OBJECT_H
#define I_OBJECT_H

#include "Shader.h"

class IObject
{
public:
	IObject() = default;
	virtual ~IObject() = default;

	virtual void draw() = 0;
};

#endif  /* I_OBJECT_H */