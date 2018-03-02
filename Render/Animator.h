#pragma once

enum AniRepeatType
{
	AniRepeatType_End = 0,
	AniRepeatType_Loop,
	AniRepeatType_Reverse,
};

struct AnimationClip
{
	DWORD time;
	Texture* texture;

	friend class Animator;

	AnimationClip()
	{
		texture = NULL;
		time = (DWORD)0.0f;
	}

	AnimationClip(Texture* texture, DWORD time)
	{
		this->texture = texture;
		this->time = time;
	}
};

class Animator
{
public:
	Animator(D3DXVECTOR2 start, vector<AnimationClip>* srcClips
		, AniRepeatType type = AniRepeatType_End);
	~Animator();

	void Play();
	void Pause();
	void Stop();

	void Update();
	void Render();

	Texture* GetCurrentClipTexture()
	{
		return clips[curClipNumber].texture;
	}

private:
	D3DXVECTOR2 point;
	vector<AnimationClip> clips;

	AniRepeatType repeatType;
	bool isReverseRight;

	bool isPlay;
	DWORD playTime;
	UINT curClipNumber;
};