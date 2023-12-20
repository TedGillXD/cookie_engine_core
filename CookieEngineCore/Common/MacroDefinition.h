#pragma once

#ifndef DISABLE_COPY
#define DISABLE_COPY(Type)						\
		explicit Type(const Type&) = delete;	\
		Type& operator=(const Type&) = delete;
#endif

#ifndef DISABLE_MOVE
#define DISABLE_MOVE(Type)						\
		explicit Type(Type&&) = delete;			\
		Type& operator=(Type&&) = delete;
#endif

#ifndef DISABLE_MOVE_AND_COPY
#define DISABLE_MOVE_AND_COPY(Type)	DISABLE_COPY(Type) DISABLE_MOVE(Type)
#endif