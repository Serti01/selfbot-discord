#pragma once

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "../vendor/json.hpp"
using nlohmann::json;

extern bool ended;

#include "api.hh"
#include "ws.hh"

#include <chrono>
#include <thread>