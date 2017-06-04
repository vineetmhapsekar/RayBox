#include <gtest\gtest.h>
#include "RayBox.hpp"
using namespace RayBox;

TEST(RayBox_InvalidConfigInpu, RayBox)
{
	RayBox	rayBox(2);

	try {
		std::shared_ptr<Mirror> mirror1 = std::make_shared<Mirror>(0, 0, 10);
		rayBox.AddMirror(mirror1);

		std::shared_ptr<Mirror> mirror2 = std::make_shared<Mirror>(0, 0, 10);
		rayBox.AddMirror(mirror2);
	}
	catch(std::logic_error& ex) {
		EXPECT_STREQ("Duplicate Mirror", ex.what());
	}
}