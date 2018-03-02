#pragma once

class Frames
{
public:
	static Frames* Get();
	static void Delete();

	void Update();
	void Print();

	void Start();
	void Stop();

	bool IsTimerStopped() const { return isTimerStopped; }
	float FramePerSecond() const { return framePerSecond; }
	float RunningTime() const { return runningTime; }
	float TimeElapsed() const { return isTimerStopped ? 0.0f : timeElapsed; }

private:
	Frames(void);
	~Frames(void);

	static Frames* instance;///< �̱��� ��ü

	LPD3DXFONT font;

	bool isTimerStopped;///< Ÿ�̸� ����

	INT64 ticksPerSecond;///< �ʴ� ƽī��Ʈ
	INT64 currentTime;///< ���� �ð�
	INT64 lastTime;///< �����ð�
	INT64 lastFPSUpdate;///< ������ FPS ������Ʈ �ð�
	INT64 fpsUpdateInterval;///< fps ������Ʈ ����
	
	UINT frameCount;///< ������ ��
	float runningTime;///< ���� �ð�
	float timeElapsed;///< ���� ���������κ��� ����ð�
	float framePerSecond;///< FPS
};