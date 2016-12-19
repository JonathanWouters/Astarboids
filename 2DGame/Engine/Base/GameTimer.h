#pragma once

class GameTimer
{
public:
	GameTimer();
	~GameTimer();
	float GetFPS();
	float DeltaTime();

private:
	//Timer can only be updated in Gamescene
	friend class GameScene;
	void Update();

	Uint64 m_LastTime = 0;
	Uint64 m_CurrentTime = 0;
	float m_DeltaTime = 0;
	float m_Fps = 0;
};
