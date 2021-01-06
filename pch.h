#pragma once

#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>
#include <fstream>
#include <variant>

template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;