// Fill out your copyright notice in the Description page of Project Settings.


#include "MagePlayerState.h"


void AMagePlayerState::AddScore(float ScoreDelta) {
    AMagePlayerState::SetScore(AMagePlayerState::GetScore() + ScoreDelta);
}