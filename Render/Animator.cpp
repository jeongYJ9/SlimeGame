#include "../stdafx.h"
#include "Animator.h"

Animator::Animator(D3DXVECTOR2 start, vector<AnimationClip>* srcClips, AniRepeatType type)
	: point(start), repeatType(type)
	, isPlay(false), curClipNumber(0), isReverseRight(true)
{
	for (size_t i = 0; i < srcClips->size(); i++)
	{
		AnimationClip clip = srcClips->at(i);
		clip.time = clip.time < 1 ? 100 : clip.time;

		clips.push_back(clip);
	}
}

Animator::~Animator()
{
	
}

void Animator::Play()
{
	isPlay = true;
	playTime = timeGetTime();
}

void Animator::Pause()
{
	isPlay = false;
}

void Animator::Stop()
{
	isPlay = false;
	curClipNumber = 0;
}

void Animator::Update()
{
	AnimationClip clip = clips[curClipNumber];

	if (timeGetTime() - playTime > clip.time)
	{
		switch (repeatType)
		{
			case AniRepeatType_End:
			{
				curClipNumber++;

				if (curClipNumber >= clips.size() - 1)
					curClipNumber = clips.size() - 1;
			}
			break;

			case AniRepeatType_Loop:
			{
				curClipNumber++;
				curClipNumber %= clips.size();
			}
			break;

			case AniRepeatType_Reverse:
			{
				if (isReverseRight == true)
				{
					curClipNumber++;

					if (curClipNumber >= clips.size() - 1)
						isReverseRight = false;
				}
				else
				{
					curClipNumber--;

					if (curClipNumber <= 0)
						isReverseRight = true;
				}
			}
			break;
		}

		playTime = timeGetTime();
	}
}

void Animator::Render()
{
	AnimationClip clip = clips[curClipNumber];
	
	clip.texture->Render();
}
