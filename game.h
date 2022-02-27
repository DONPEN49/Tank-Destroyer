#pragma once
#include <cinttypes>
#include <DirectXMath.h>

void GameInit();
void GameInput(uint64_t dt);
void GameUpdate(uint64_t dt);
void GameRender(uint64_t dt);
void GameDispose();

