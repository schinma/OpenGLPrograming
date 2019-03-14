#pragma once
class IObject
{
public:
	IObject() = default;
	virtual ~IObject() = default;

	virtual void draw() = 0;
};

