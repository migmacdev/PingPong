#pragma once
class Score {
public:
	int scoreP1;
	int scoreP2;
	int ScoreToWin;

	void reset();
	void start();
	void addTo(int);
};