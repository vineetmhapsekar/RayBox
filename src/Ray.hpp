#ifndef RAY_HPP
#define RAY_HPP

namespace RayBox {

	/**
	 * @brief      { Class Ray for storing ray current direction and current co-ordinates }
	 */
	struct Ray {
		enum class Direction {
			LeftToRight							= 0,
			RightToLeft,
			BottomToTop,
			TopToBottom,
			Straight
		};

		int										_column;
		int										_row;
		Direction								_direction;
	};

}

#endif //RAY_HPP