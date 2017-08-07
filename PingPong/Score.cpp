#include "stdafx.h"
#include "Score.h"

void Score::reset()
{
}

void Score::start()
{
}

void Score::addTo(int playerNum) {
	if (playerNum == 1) {
		scoreP1++;
	}
	else {
		scoreP2++;
	}
}