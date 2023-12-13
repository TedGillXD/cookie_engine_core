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
		[[nodiscard]] bool Contain(Type data);
		void Erase(Type data);
		void Clear();

		inline const uint32_t GetSize() const { return _size; }
		inline const uint32_t GetBucketSize() const { return _buckets.GetSize(); }

	public:
		// implement class Iterator, begin() and end() to support C++11 interation
		class Iterator {
		public:
			Iterator(Type* ptr) : _ptr(ptr), currentBucketIndex(0), currentIndex(0) { }
			Iterator(Type* ptr, uint32_t bucketIndex) : _ptr(ptr), currentBucketIndex(bucketIndex), currentIndex(0) { }

			[[nodiscard]] inline Type& operator*() const { return *_ptr[currentBucketIndex][currentIndex]; }
			[[nodiscard]] inline Iterator& operator++() { 
				currentIndex++; 
				if (currentIndex >= *_ptr[currentBucketIndex].GetSize()) {
					currentIndex = 0;
					currentBucketIndex++;
				}
			}
			[[nodiscard]] inline bool operator!=(const Iterator& other) const { 
				return (_ptr != other._ptr || currentBucketIndex != other.currentBucketIndex || currentIndex != other.currentIndex);
			}
			[[nodiscard]] inline bool operator==(const Iterator& other) const {
				return (_ptr == other._ptr && currentBucketIndex == other.currentBucketIndex && currentIndex == other.currentIndex);
			}
		private:
			Array<Array<Type>>* _ptr;
			uint32_t currentBucketIndex;
			uint32_t currentIndex;
		};

		inline Iterator begin() { return Iterator(&_buckets); }
		inline Iterator end() { return Iterator(&_buckets, _buckets.GetSize()); }
		inline Iterator begin() const { return Iterator(&_buckets); }
		inline Iterator end() const { return Iterator(&_buckets, _buckets.GetSize()); }

	private:
		static const uint32_t defaultBucketSize = 13;

		Array<Array<Type>> _buckets;
		uint32_t _size = 0;

#ifdef _DEBUG
	private:
		std::string _name;
#endif
	};

	// member function definitions of HashSet
	template<typename Type>
	HashSet<Type>::HashSet() {
		_buckets.Resize(defaultBucketSize);
		Init();
	}

	template<typename Type>
	HashSet<Type>::HashSet(uint32_t bucketCount) {
		_buckets.Resize(bucketCount);
		Init();
	}

	template<typename Type>
	HashSet<Type>::HashSet(Array<Type>& arr) {
		_buckets.Resize(defaultBucketSize);
		Init();
		for (Type& item : arr) {
			Insert(item);
		}
	}

	template<typename Type>
	HashSet<Type>::HashSet(std::initializer_list<Type> initList) {
		_buckets.Resize(defaultBucketSize);
		Init();
		for (Type& item : initList) {
			Insert(item);
		}
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
	}

	template<typename Type>
	bool HashSet<Type>::Contain(Type data) {
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

}