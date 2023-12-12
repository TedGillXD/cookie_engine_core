#pragma once
#include <cstdint>
#include <cassert>
#include <cstring>
#include <algorithm>

namespace Cookie::Util {

	template<typename Type, bool bShouldDestruct = false>
	class Array {
	public:		// constructors and destructors
		//constructors
		Array();
		Array(uint32_t size);
		Array(uint32_t size, Type data);
		~Array();									//destructor
		Array(const Array& other);					//copy constructor
		Array& operator=(const Array& other);		//copy assignment operator
		Array(Array&& other) noexcept;				//move constructor
		Array& operator=(Array&& other) noexcept;	//move assignment operator

	private:
		inline void HandleInsufficientCapacity();
		inline void Reset();

	public:		// member functions
		void PushBack(Type data);
		void PopBack();
		void Insert(Type data, uint32_t index);
		void Erase(uint32_t index);
		void Resize(uint32_t newSize);

		[[nodiscard]] Type& Front();
		[[nodiscard]] Type& Back();
		[[nodiscard]] Type& At(uint32_t index);
		[[nodiscard]] Type& operator[](uint32_t index);

		void Reserve(uint32_t newCapacity);

		[[nodiscard]] inline uint32_t GetCapacity();
		[[nodiscard]] inline uint32_t GetSize();
		[[nodiscard]] inline Type* GetData();

		void Clear();

		//to support range interation in C++11
		[[nodiscard]] constexpr Type* begin();
		[[nodiscard]] constexpr const Type* begin() const;
		[[nodiscard]] constexpr Type* end();
		[[nodiscard]] constexpr const Type* end() const;

	private:	// member variables
		Type* _data;
		uint32_t _capacity;
		uint32_t _size;
	};

	// Definition of class Array
	// constructors
	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>::Array() {
		_capacity = 10;
		_size = 0;
		_data = new Type[_capacity];
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>::Array(uint32_t size) {
		_capacity = size;
		_size = size;
		_data = new Type[size];
		memset(_data, 0, sizeof(_data));
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>::Array(uint32_t size, Type data) : Array(size) {
		std::fill(_data, _data + size, &data);
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>::~Array() {
		if (bShouldDestruct) {
			std::for_each(_data, _data + _size, [](Type& data) -> void { data.~Type(); });
		}
		delete[] _data;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>::Array(const Array& other) : _size(other._size), _capacity(other._capacity) {
		_data = new Type[_size];
		for (uint32_t i = 0; i < _size; i++) {
			_data[i] = other._data[i];
		}
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>& Array<Type, bShouldDestruct>::operator=(const Array& other) {
		assert(this != std::addressof(other));
		if (this != std::addressof(other)) {
			Clear();
			Reserve(other._size);
			for (uint32_t i = 0; i < _size; i++) {
				_data[i] = other._data[i];
			}
		}
		return *this;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>::Array(Array&& other) noexcept : _data(other._data), _capacity(other._capacity), _size(other._size) {
		other.Reset();
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Array<Type, bShouldDestruct>& Array<Type, bShouldDestruct>::operator=(Array&& other) noexcept {
		assert(this != std::addressof(other));
		if (this != std::addressof(other)) {
			Clear();
			_capacity = 0;
			if(_data) delete[] _data;
			_data = nullptr;

			_capacity = other._capacity;
			_size = other._size;
			_data = other._size;
			other.Reset();
		}
		return *this;
	}


	//private function
	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::HandleInsufficientCapacity()
	{
		_capacity *= 2;
		auto* temp = new Type[_capacity];
		memcpy(temp, _data, sizeof(_data));
		if(_data) delete[] _data;
		_data = temp;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::Reset() {
		_capacity = 0;
		_size = 0;
		_data = nullptr;
	}


	//public member functions
	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::PushBack(Type data) {
		//handle data size >= capacity
		if (_size >= _capacity) {
			HandleInsufficientCapacity();
		}
		assert(_size < _capacity);

		_data[_size] = data;
		_size++;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::PopBack() {
		assert(_data && _size);
		_size--;
		if (bShouldDestruct) {
			_data[_size].~Type();
		}
		else {
			memset(_data[_size], 0, sizeof(Type));
		}
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::Insert(Type data, uint32_t index) {
		assert(_data && _size);
		if (_size >= _capacity) {
			HandleInsufficientCapacity();
		}

		auto* item = std::addressof(_data[index]);
		auto* nextItem = item + 1;
		memcpy(
			nextItem,
			item,
			(std::addressof(_data[_size]) - item) * sizeof(Type)
		);
		_data[index] = data;
		_size++;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::Erase(uint32_t index) {
		assert(_data && _size && index < _size);

		auto* item = std::addressof(_data[index]);
		auto* nextItem = item + 1;
		if (bShouldDestruct) item->~Type();
		memcpy(
			item,
			nextItem,
			(std::addressof(_data[_size]) - nextItem) * sizeof(Type)
		);
		_size--;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::Resize(uint32_t newSize) {
		if (newSize < _size) {
			if (bShouldDestruct) {	// cleaning
				std::for_each(_data + newSize, _data + _size, [](Type& item) -> void { item.~Type(); });
			}
			else {
				memset(_data + newSize, 0, sizeof(Type) * (_size - newSize));
			}
		}
		_size = newSize;
		while (_size > _capacity) {	// if newsize is bigger than capacity, we need to allocate more space
			HandleInsufficientCapacity();
		}
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Type& Array<Type, bShouldDestruct>::Front() {
		assert(_data && _size);
		return _data[0];
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Type& Array<Type, bShouldDestruct>::Back() {
		assert(_data && _size);
		return _data[_size - 1];
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Type& Array<Type, bShouldDestruct>::At(uint32_t index) {
		assert(_data && _size && index < _size);
		return _data[index];
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Type& Array<Type, bShouldDestruct>::operator[](uint32_t index) {
		assert(_data && _size && index < _size);
		return _data[index];
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::Reserve(uint32_t newCapacity) {
		if (newCapacity < _size) {
			Resize(newCapacity);
		}
		else if (newCapacity > _size) {
			auto* temp = new Type[newCapacity];
			memcpy(temp, _data, sizeof(_data));
			delete[] _data;
			_data = temp;
		}
		_capacity = newCapacity;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	uint32_t Array<Type, bShouldDestruct>::GetCapacity() {
		return _capacity;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	Type* Array<Type, bShouldDestruct>::GetData() {
		return _data;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	uint32_t Array<Type, bShouldDestruct>::GetSize() {
		return _size;
	}


	template<typename Type, bool bShouldDestruct /*= false*/>
	void Array<Type, bShouldDestruct>::Clear() {
		if (bShouldDestruct) {
			std::for_each(_data, _data + _size, [](Type& item) -> void { item.~Type(); });
		}
		_size = 0;
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	constexpr const Type* Array<Type, bShouldDestruct>::begin() const {
		return std::addressof(_data[0]);
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	constexpr Type* Array<Type, bShouldDestruct>::begin() {
		return std::addressof(_data[0]);
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	constexpr const Type* Array<Type, bShouldDestruct>::end() const {
		assert(!(_data == nullptr && _size > 0));
		return std::addressof(_data[_size]);
	}

	template<typename Type, bool bShouldDestruct /*= false*/>
	constexpr Type* Array<Type, bShouldDestruct>::end() {
		assert(!(_data == nullptr && _size > 0));
		return std::addressof(_data[_size]);
	}

}
