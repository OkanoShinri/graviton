#include "BackGroundImage.h"

BackGroundImage::BackGroundImage()
{
	images.emplace_back(std::make_unique<ofImage>());
	images.back()->load("ga08-5.png");
}

void BackGroundImage::updata()
{
}

void BackGroundImage::draw()
{
	ofSetColor(255, 255, 255, 20);
	images.back()->draw(0, 0);
}

void BackGroundImage::changeImage()
{
}
