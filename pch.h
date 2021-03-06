#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <sstream>
#include <fstream>
#include <variant>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

#define DELETE_VEC(VEC)                    \
do {                                       \
	for(auto item : VEC)				   \
		delete item;					   \
} while (0)