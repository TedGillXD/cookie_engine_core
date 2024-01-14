#pragma once
#include "Array.hpp"
#include <cassert>
#include <string>
#include <functional>
#include <initializer_list>

namespace Cookie::Util {

	template<typename Type>
	class Set {

	};

	template<typename Type>
	class HashSet {
		static_assert(std::is_default_constructible<Type>::value, "Type should be default constructible!");

		DEBUG_INFO()

	public:	//constructors and destructor
		//constructors
		HashSet();
		HashSet(uint32_t bucketCount);
		HashSet(Array<Type>& arr);
		HashSet(std::initializer_list<Type> initList);
		HashSet(const HashSet& other);					//copy constructor
		HashSet& operator=(const HashSet& other);		//copy assignment operator
		HashSet(HashSet&& other) noexcept;				//move constructor
		HashSet& operator=(HashSet&& other) noexcept;	//move assignment operator
		~HashSet() = default;							//destructor

	private:	//internal member functions
		inline uint32_t GetHashKey(const Type& data);
		inline void Reset();
		inline void Init();

	public:	//member functions
		void Insert(Type data);
		[[nodiscard]] bool Contain(const Type& data);
		void Erase(Type data);
		void Clear();

		inline const uint32_t GetSize() const { return _size; }
		inline const uint32_t GetBucketSize() const { return _buckets.GetSize(); }

	public:
		// implement class Iterator, begin() and end() to support C++11 interation
		class Iterator {
		public:
			Iterator(Array<Array<Type>>* ptr) : _ptr(ptr), currentBucketIndex(-1), currentIndex(0) { 
				currentBucketIndex = GetNextAvailable(currentBucketIndex);
			}
			Iterator(Array<Array<Type>>* ptr, uint32_t bucketIndex) : _ptr(ptr), currentBucketIndex(bucketIndex), currentIndex(0) { }

			inline Type& operator*() const { return (*_ptr)[currentBucketIndex][currentIndex]; }
			inline Iterator& operator++() { 
				currentIndex++; 
				if (currentIndex >= (*_ptr)[currentBucketIndex].GetSize()) {
					currentIndex = 0;
					currentBucketIndex = GetNextAvailable(currentBucketIndex);
				}
				return *this;
			}
			inline bool operator!=(const Iterator& other) const { 
				return (_ptr != other._ptr || currentBucketIndex != other.currentBucketIndex || currentIndex != other.currentIndex);
			}
			inline bool operator==(const Iterator& other) const {
				return (_ptr == other._ptr && currentBucketIndex == other.currentBucketIndex && currentIndex == other.currentIndex);
			}

		private:
			[[nodiscard]] inline uint32_t GetNextAvailable(uint32_t current) {
				current++;
				while (current < (*_ptr).GetSize()) {
					if ((*_ptr)[current].GetSize() == 0) {
						current++;
					} else {
						break;
					}
				}
				return current;
			}

		private:
			Array<Array<Type>>* _ptr;
			uint32_t currentBucketIndex;
			uint32_t currentIndex;
		};

		[[nodiscard]] inline Iterator begin() { return Iterator(&_buckets); }
		[[nodiscard]] inline Iterator end() { return Iterator(&_buckets, _buckets.GetSize()); }
		[[nodiscard]] Iterator begin() const { return Iterator(&_buckets); }
		[[nodiscard]] inline Iterator end() const { return Iterator(&_buckets, _buckets.GetSize()); }

	private:
		void Expansion();

	private:
		inline static const uint32_t defaultBucketSize = 13;
		inline static const float loadFactor = 0.75f;

		Array<Array<Type>> _buckets;
		uint32_t _size = 0;
	};

	// member function definitions of HashSet
	template<typename Type>
	HashSet<Type>::HashSet() {
		_buckets.Resize(defaultBucketSize);
		Init();
		OBJECT_CREATE("HashSet");
	}

	template<typename Type>
	HashSet<Type>::HashSet(uint32_t bucketCount) {
		_buckets.Resize(bucketCount);
		Init();
		OBJECT_CREATE("HashSet");
	}

	template<typename Type>
	HashSet<Type>::HashSet(Array<Type>& arr) {
		int bucketSize = (int)std::ceil((float)arr.GetSize() / loadFactor);
		_buckets.Resize(bucketSize);
		Init();
		for (Type& item : arr) {
			Insert(item);
		}
		OBJECT_CREATE("HashSet");
	}

	template<typename Type>
	HashSet<Type>::HashSet(std::initializer_list<Type> initList) {
		int bucketSize = (int)std::ceil((float)initList.size() / loadFactor);
		_buckets.Resize(bucketSize);
		Init();
		for (const Type& item : initList) {
			Insert(item);
		}
		OBJECT_CREATE("HashSet");
	}

	template<typename Type>
	HashSet<Type>::HashSet(const HashSet& other) {
		_buckets.Clear();
		_buckets.Resize(other._buckets.GetSize());
		Init();
		uint32_t newSize = 0;
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			_buckets[i] = other._buckets[i];
			newSize += other._buckets[i].GetSize();
		}
		_size = newSize;
		OBJECT_CREATE("HashSet");
	}

	template<typename Type>
	HashSet<Type>& HashSet<Type>::operator=(const HashSet& other) {
		assert(this != std::addressof(other));
		if (this != std::addressof(other)) {
			_buckets.Clear();
			_buckets.Resize(other._buckets.GetSize());
			Init();
			uint32_t newSize = 0;
			for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
				this->_buckets[i] = other._buckets[i];
				newSize += other._buckets[i].GetSize();
			}
			_size = newSize;
		}
		OBJECT_CREATE("HashSet");
		return *this;
	}

	template<typename Type>
	HashSet<Type>::HashSet(HashSet&& other) noexcept {
		_buckets.Clear();
		_buckets.Resize(other._buckets.GetSize());
		Init();
		uint32_t newSize = 0;
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			_buckets[i] = other._buckets[i];
			newSize += other._buckets[i].GetSize();
		}
		_size = newSize;
		other.Reset();
		OBJECT_CREATE("HashSet");
	}

	template<typename Type>
	HashSet<Type>& HashSet<Type>::operator=(HashSet&& other) noexcept {
		assert(this != std::addressof(other));
		if (this != std::addressof(other)) {
			_buckets.Clear();
			_buckets.Resize(other._buckets.GetSize());
			Init();
			uint32_t newSize = 0;
			for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
				_buckets[i] = other._buckets[i];
				newSize += other._buckets[i].GetSize();
			}
			_size = newSize;
			other.Reset();
		}
		OBJECT_CREATE("HashSet");
		return *this;
	}

	template<typename Type>
	uint32_t HashSet<Type>::GetHashKey(const Type& data) {
		std::hash<Type> hasher;
		return hasher(data) % _buckets.GetSize();
	}

	template<typename Type>
	void HashSet<Type>::Reset() {
		_buckets.Clear();
		_size = 0;
	}

	template<typename Type>
	void HashSet<Type>::Init() {
		uint32_t size = _buckets.GetSize();
		for (uint32_t i = 0; i < size; i++) {
			_buckets[i] = Array<Type>();
		}
	}

	template<typename Type>
	void HashSet<Type>::Insert(Type data) {
		if (Contain(data)) return;
		_buckets[GetHashKey(data)].PushBack(data);
		_size++;

		if ((float)_buckets.GetSize() * 0.75f < _size) {
			Expansion();
		}
	}

	template<typename Type>
	bool HashSet<Type>::Contain(const Type& data) {
		for (Type& item : _buckets[GetHashKey(data)]) {
			if (item == data) {
				return true;
			}
		}
		return false;
	}

	template<typename Type>
	void HashSet<Type>::Erase(Type data) {
		if (!Contain(data)) return;
		Array<Type>& tempArr = _buckets[GetHashKey(data)];
		for (uint32_t i = 0; i < tempArr.GetSize(); i++) {
			if (tempArr[i] == data) {
				tempArr.Erase(i);
				break;
			}
		}
		_size--;
	}

	template<typename Type>
	void HashSet<Type>::Clear() {
		_buckets.Clear();
		Init();
	}

	template<typename Type>
	void HashSet<Type>::Expansion() {
		int newBucketSize = _buckets.GetSize();
		while ((float)newBucketSize * 0.75f <= _size) {
			newBucketSize *= 2;
		}

		Array<Array<Type>> newBucket(newBucketSize, Array<Type>());
		for (uint32_t i = 0; i < _buckets.GetSize(); i++) {
			for (uint32_t j = 0; j < _buckets.At(i).GetSize(); j++) {
				newBucket.At(i).PushBack(_buckets.At(i).At(j));
			}
		}

		_buckets.Clear();
		_buckets.~Array();
		_buckets = newBucket;
	}

}