#ifndef MIRROR_HPP
#define MIRROR_HPP

#include <vector>
#include <memory>
#include "Ray.hpp"

namespace RayBox {

	/**
	 * @brief      Mirror class for storing mirror details, providing deflection mechanism.
	 */

	class Mirror {
	public:

		/**
		 * @brief      Enum for providing result of deflection due to mirror.
		 */
		enum class DeflectionResult {
			undefined									= -1,
			Hit											= 0,
			Deflected,
			Evaporated
		};

		Mirror() = default;
		~Mirror() = default;

		/**
		 * @brief      { Constructor for initializing mirror with its co-ordinates and strength }
		 *
		 * @param[in]  row         The row Index
		 * @param[in]  column      The column Index
		 * @param[in]  strength    The strength
		 * @param[in]  deflection  The deflection Angle
		 */
		Mirror(const int row, const int column, const int strength = 0, const int deflection = 0)
			: _columnIndex(column), _rowIndex(row), _strength(strength), 
			_deflectionAngle(deflection) {
		}

		Mirror(const Mirror& mirror) {
			this->_columnIndex							= mirror._columnIndex;
			this->_rowIndex								= mirror._rowIndex;
			this->_strength								= mirror._strength;
			this->_deflectionAngle						= mirror._deflectionAngle;
		}

		/**
		 * @brief      { Provides function for deflecting ray to correct path, on mirror collision }
		 *
		 * @param      ray   The ray
		 *
		 * @return     { Enum for providing result of deflection due to mirror. }
		 */
		DeflectionResult deflectRay(Ray& ray) {
			switch (_deflectionAngle)
			{
			case -90:
				deflectRayByNeg90(ray);
				return DeflectionResult::Deflected;
			case 90:
				deflectRayByPos90(ray);
				return DeflectionResult::Deflected;
			case -180:
			case 180:
				deflectRayBy180(ray);
				return DeflectionResult::Deflected;
			case 0:
				if (_strength > 0) {
					decreaseStrength();
					if (_strength == 0)
						return DeflectionResult::Evaporated;
				}
				return DeflectionResult::Hit;
			}
			return DeflectionResult::undefined;
		}

		inline int getColumnIndex() {
			return _columnIndex;
		}

		inline int getRowIndex() {
			return _rowIndex;
		}

		inline int getdeflectionAngle() {
			return _deflectionAngle;
		}

		inline int getStrength() {
			return _strength;
		}

		inline void setColumnIndex(int index) {
			_columnIndex							= index;
		}

		inline void setRowIndex(int index) {
			_rowIndex								= index;	
		}

		inline void setDeflectionAngle(int angle) {
			_deflectionAngle						= angle;
		}

	private:

		void deflectRayByNeg90(Ray& ray) {

			switch (ray._direction)
			{
			case Ray::Direction::LeftToRight:
			case Ray::Direction::RightToLeft: {
				ray._direction						= Ray::Direction::BottomToTop;
				ray._row							-= 1;
			}
			break;
			case Ray::Direction::TopToBottom:
			case Ray::Direction::BottomToTop: {
				ray._direction						= Ray::Direction::LeftToRight;
				ray._column							+= 1;
			}
			break;
			default:
				std::cout << "Invalid direction in PassTheRay" << std::endl;
				return ;

			}
		}

		void deflectRayByPos90(Ray& ray) {
			switch (ray._direction)
			{
			case Ray::Direction::LeftToRight:
			case Ray::Direction::RightToLeft: {
				ray._direction						= Ray::Direction::TopToBottom;
				ray._row							+= 1;
			}
			break;
			case Ray::Direction::TopToBottom:
			case Ray::Direction::BottomToTop: {
				ray._direction						= Ray::Direction::RightToLeft;
				ray._column							-= 1;
			}
			break;
			default:
				std::cout << "Invalid direction in PassTheRay" << std::endl;
				return ;
			}
		}

		void deflectRayBy180(Ray& ray) {
			switch (ray._direction) {
			case Ray::Direction::LeftToRight: {
				ray._direction						= Ray::Direction::RightToLeft;
				ray._column							-= 1;
			}
			break;
			case Ray::Direction::RightToLeft: {
				ray._direction						= Ray::Direction::LeftToRight;
				ray._column							+= 1;
			}
			break;
			case Ray::Direction::TopToBottom: {
				ray._direction						= Ray::Direction::BottomToTop;
				ray._row							+= 1;
			}
			break;
			case Ray::Direction::BottomToTop: {
				ray._direction						= Ray::Direction::TopToBottom;
				ray._row							-= 1;
			}
			break;
			default:
				std::cout << "Invalid direction in PassTheRay" << std::endl;
				return ;
			}
		}

		
		void decreaseStrength() {
			if (_strength > 0) {
				--_strength;
			}
		}

	private:
		int											_columnIndex;
		int											_rowIndex;
		int											_strength;
		int											_deflectionAngle;
	};

	typedef std::vector<std::shared_ptr<Mirror>>	MirrorList;

}

#endif //MIRROR_HPP