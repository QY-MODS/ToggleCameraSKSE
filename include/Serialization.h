#pragma once

#include "Settings.h"


void SaveCallback(SKSE::SerializationInterface* serializationInterface);

[[maybe_unused]] void LoadCallback(SKSE::SerializationInterface* serializationInterface);

void InitializeSerialization();