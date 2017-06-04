#ifndef REYBOX_HPP
#define REYBOX_HPP

#include <iostream>
#include "Ray.hpp"
#include "Mirror.hpp"

namespace RayBox {

	/**
	 * @brief      Class for raybox.
	 * 				This class covers majority of processing ray using list of Mirrors.
	 * 				Specification of Game is given in pdf file name : reybox 3.pdf
	 */		
	class Raybox {

	public:
		Raybox(const int columns) : _maxColumns(columns), _mirrors(std::vector<std::shared_ptr<Mirror>>(columns*columns)),
			_rowRefMirrorList(std::vector<MirrorList>(columns)), _colRefMirrorList(std::vector<MirrorList>(columns)) {
		}

		/**
		 * @brief      { This function passes the array according to direction of array. 
		 * 				Ray parameter contains current direction and co-ordinates of Ray. }
		 *
		 * @param      ray   The ray
		 */
		void PassTheRay(Ray& ray) noexcept {

			switch (ray._direction)
			{
			case Ray::Direction::LeftToRight:
				return PassFromLeftToRight(ray);
			case Ray::Direction::RightToLeft:
				return PassFromRightToLeft(ray);
			case Ray::Direction::TopToBottom:
				return PassFromTopToBottom(ray);
			case Ray::Direction::BottomToTop:
				return PassFromBottomToTop(ray);
			default:
				std::cout << "Invalid direction in PassTheRay" << std::endl;
				return ;
			}
		}

		/**
		 * @brief      Adds a mirror to list of Mirrors. 
		 * 				This function also adds reference mirror which are in adjacent diagonal sides of mirror.
		 *
		 * @param[in]  mirror  The mirror
		 */
		void AddMirror(std::shared_ptr<Mirror> mirror) throw(std::logic_error) {

			/// Original Mirror with 0 deflection, absorbing ray 
			std::shared_ptr<Mirror> ref = _mirrors[(mirror->getRowIndex() * _maxColumns) + mirror->getColumnIndex()];
			if (ref.get() == nullptr) {
				_mirrors[(mirror->getRowIndex() * _maxColumns) + mirror->getColumnIndex()] = mirror;
			}
			else {
				std::cout << "Mirror: Row: " << ref->getRowIndex() << " Column: " << ref->getColumnIndex() << std::endl;
				throw std::logic_error("Duplicate Mirror");
			}

			/// Reference Mirror at top left diagonally adjacent column with 90 degree deflection 
			if ((mirror->getRowIndex() - 1) > -1 && (mirror->getColumnIndex() - 1) > -1) {
				ref = _mirrors[((mirror->getRowIndex() - 1) * _maxColumns) + mirror->getColumnIndex() - 1];
				if (ref.get() != nullptr) {
					ref->setDeflectionAngle(ref->getdeflectionAngle() + -90);
				}
				else {
					std::shared_ptr<Mirror> ref1 = std::make_shared<Mirror>(*mirror);
					ref1->setDeflectionAngle(-90);
					ref1->setColumnIndex(mirror->getColumnIndex() - 1);
					ref1->setRowIndex(mirror->getRowIndex() - 1);
					_mirrors[(ref1->getRowIndex() * _maxColumns) + ref1->getColumnIndex()] = ref1;
				}
			}

			/// Reference Mirror at top right diagonally adjacent column with 90 degree deflection
			if ((mirror->getRowIndex() - 1) > -1 && (mirror->getColumnIndex() + 1) < _maxColumns) {
				ref = _mirrors[((mirror->getRowIndex() - 1) * _maxColumns) + mirror->getColumnIndex() + 1];
				if (ref.get() != nullptr) {
					ref->setDeflectionAngle(ref->getdeflectionAngle() + -90);
				}
				else {
					std::shared_ptr<Mirror> ref2 = std::make_shared<Mirror>(*mirror);
					ref2->setDeflectionAngle(-90);
					ref2->setColumnIndex(mirror->getColumnIndex() + 1);
					ref2->setRowIndex(mirror->getRowIndex() - 1);
					_mirrors[(ref2->getRowIndex() * _maxColumns) + ref2->getColumnIndex()] = ref2;
				}
			}

			/// Reference Mirror at bottom left diagonally adjacent column with 90 degree deflection 
			if ((mirror->getRowIndex() + 1) < _maxColumns && (mirror->getColumnIndex() - 1) > -1) {
				ref = _mirrors[((mirror->getRowIndex() + 1) * _maxColumns) + mirror->getColumnIndex() - 1];
				if (ref.get() != nullptr) {
					ref->setDeflectionAngle(ref->getdeflectionAngle() + 90);
				}
				else {
					std::shared_ptr<Mirror> ref3 = std::make_shared<Mirror>(*mirror);
					ref3->setDeflectionAngle(90);
					ref3->setColumnIndex(mirror->getColumnIndex() - 1);
					ref3->setRowIndex(mirror->getRowIndex() + 1);
					_mirrors[(ref3->getRowIndex() * _maxColumns) + ref3->getColumnIndex()] = ref3;
				}
			}

			/// Reference Mirror at bottom right diagonally adjacent column with 90 degree deflection 
			if ((mirror->getRowIndex() + 1) < _maxColumns && (mirror->getColumnIndex() + 1) < _maxColumns) {
				ref = _mirrors[((mirror->getRowIndex() + 1) * _maxColumns) + mirror->getColumnIndex() + 1];
				if (ref.get() != nullptr) {
					ref->setDeflectionAngle(ref->getdeflectionAngle() + 90);
				}
				else {
					std::shared_ptr<Mirror> ref4 = std::make_shared<Mirror>(*mirror);
					ref4->setDeflectionAngle(90);
					ref4->setColumnIndex(mirror->getColumnIndex() + 1);
					ref4->setRowIndex(mirror->getRowIndex() + 1);
					_mirrors[(ref4->getRowIndex() * _maxColumns) + ref4->getColumnIndex()] = ref4;
				}
			}
		}

		/**
		 * @brief      { Simple function to print the RayBox with Mirror location }
		 *
		 * @param      out   Ostream 
		 */
		void print(std::ostream& out) {
			for (auto itr : _mirrors) {
				if (itr.get() != nullptr)
					out << itr->getRowIndex() << "," << itr->getColumnIndex() << "," 
					<< itr->getStrength() << "," << static_cast<int>(itr->getdeflectionAngle())
					<< std::endl;
			}
		}

		/**
		 * @brief      { Manages references to MirrorList, so that processing can be improved. }
		 */
		void initReferences() {
			initRowReferences(_rowRefMirrorList);
			initColReferences(_colRefMirrorList);
		}

		inline int getSize() {
			return _maxColumns;
		}

	private:
		
		/**
		 * @brief      { Deletion of mirror when mirror strength reduces to zero }
		 *
		 * @param[in]  rowIndex  The row index
		 * @param[in]  colIndex  The col index
		 */
		void deleteMirror(int rowIndex, int colIndex) {
			_mirrors[(rowIndex * _maxColumns) + colIndex].reset();
			MirrorList list = _rowRefMirrorList[rowIndex];
			MirrorList::iterator itrRef = list.end();
			for (auto itr = list.begin(); itr != list.end(); itr++) {
				if (itr->get() != nullptr && (*itr)->getColumnIndex() == colIndex) {
					(*itr).reset();
					itrRef = itr;
				}
			}
			if (itrRef != list.end()) {
				list.erase(itrRef);
				_rowRefMirrorList[rowIndex] = list;
			}

			list = _colRefMirrorList[colIndex];
			itrRef = list.end();
			for (auto itr = list.begin(); itr != list.end(); itr++) {
				if (itr->get() != nullptr && (*itr)->getRowIndex() == rowIndex) {
					(*itr).reset();
					itrRef = itr;
				}
			}
			if (itrRef != list.end()) {
				list.erase(itrRef);
				_colRefMirrorList[colIndex] = list;
			}
		}

		/**
		 * @brief      { Helps in executing deflection strategy }
		 *
		 * @param      mirror  The mirror
		 * @param      ray     The ray
		 */
		void deflectMirror(std::shared_ptr<Mirror>& mirror, Ray& ray) {
			Mirror::DeflectionResult ret = mirror->deflectRay(ray);
			if (ret == Mirror::DeflectionResult::Deflected)
				PassTheRay(ray);
			else if (ret == Mirror::DeflectionResult::Hit)
				std::cout << "{" << ray._row + 1 << "," << ray._column + 1 << "}" << std::endl;
			else if (ret == Mirror::DeflectionResult::Evaporated) {
				std::cout << "{" << ray._row + 1 << "," << ray._column + 1 << "}" << std::endl;
				deleteMirror(mirror->getRowIndex(), ray._column);
			}
		}

		/**
		 * @brief      { Processing row as per Top to Bottom direction. }
		 *
		 * @param      ray   The ray
		 */
		void PassFromTopToBottom(Ray& ray) noexcept {
			MirrorList list = _colRefMirrorList[ray._column];
			if (list.size() > 0) {
				auto itr = list.cbegin();
				for (; itr != list.cend(); itr++) {
					std::shared_ptr<Mirror> mirror = *itr;
					if (mirror.get() != nullptr) {
						if (mirror->getRowIndex() < ray._row)
							continue;
						ray._row = mirror->getRowIndex();
						deflectMirror(mirror, ray);
						break;
					}
				}
				if (itr == list.end())
					std::cout << "{" << _maxColumns << "," << ray._column + 1 << "}" << std::endl;
			}
			else
				std::cout << "{" << _maxColumns << "," << ray._column + 1 << "}" << std::endl;
		}

		/**
		 * @brief      { Processing row as per Bottom to Top direction. }
		 *
		 * @param      ray   The ray
		 */
		void PassFromBottomToTop(Ray& ray) noexcept  {
			MirrorList list = _colRefMirrorList[ray._column];
			if (list.size() > 0) {
				auto itr = list.crbegin();
				for (; itr != list.crend(); itr++) {
					std::shared_ptr<Mirror> mirror = *itr;
					if (mirror.get() != nullptr) {
						if (mirror->getRowIndex() > ray._row)
							continue;
						ray._row = mirror->getRowIndex();
						deflectMirror(mirror, ray);
						break;
					}
				}
				if (itr == list.rend())
					std::cout << "{" << 0 << "," << ray._column + 1 << "}" << std::endl;
			}
			else
				std::cout << "{" << 0 << "," << ray._column + 1 << "}" << std::endl;
		}

		/**
		 * @brief      { Processing row as per Left to Right direction.  }
		 *
		 * @param      ray   The ray
		 */
		void PassFromLeftToRight(Ray& ray) noexcept  {
			MirrorList list = _rowRefMirrorList[ray._row];
			if (list.size() > 0) {
				auto itr = list.cbegin();
				for (; itr != list.cend(); itr++) {
					std::shared_ptr<Mirror> mirror = *itr;
					if (mirror.get() != nullptr) {
						if (mirror->getColumnIndex() < ray._column)
							continue;
						ray._column = mirror->getColumnIndex();
						deflectMirror(mirror, ray);
						break;
					}
				}
				if (itr == list.end())
					std::cout << "{" << ray._row + 1 << "," << _maxColumns << "}" << std::endl;
			}
			else
				std::cout << "{" << ray._row + 1 << "," << _maxColumns << "}" << std::endl;
		}

		/**
		 * @brief      { Processing row as per Right to Left direction.  }
		 *
		 * @param      ray   The ray
		 */
		void PassFromRightToLeft(Ray& ray) noexcept  {
			MirrorList list = _rowRefMirrorList[ray._row];
			if (list.size() > 0) {
				auto itr = list.crbegin();
				for (; itr != list.crend(); itr++) {
					std::shared_ptr<Mirror> mirror = *itr;
					if (mirror.get() != nullptr) {
						if (mirror->getColumnIndex() > ray._column)
							continue;
						ray._column = mirror->getColumnIndex();
						deflectMirror(mirror, ray);
						break;
					}
				}
				if (itr == list.rend())
					std::cout << "{" << ray._row + 1 << "," << 0 << "}" << std::endl;
			}
			else
				std::cout << "{" << ray._row + 1 << "," << 0 << "}" << std::endl;
		}

		void initRowReferences(std::vector<MirrorList>& list) {
			for (auto itr : _mirrors) {
				if (itr.get() != nullptr) {
					list[itr->getRowIndex()].push_back(itr);
				}
			}
		}

		void initColReferences(std::vector<MirrorList>& list) {
			for (auto itr : _mirrors) {
				if (itr.get() != nullptr) {
					list[itr->getColumnIndex()].push_back(itr);
				}
			}
		}

	private:
		int													_maxColumns;
		MirrorList											_mirrors;
		std::vector<MirrorList>								_rowRefMirrorList;
		std::vector<MirrorList>								_colRefMirrorList;
	};

}


#endif //REYBOX_HPP
