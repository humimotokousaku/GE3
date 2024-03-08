#pragma once
#include "Vector3.h"

#define XAUDIO2_HELPER_FUNCTIONS 1
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>
#pragma comment(lib, "xaudio2.lib")
#include <fstream>
#include <wrl.h>
#include <list>


// チャンクヘッダ
struct ChunkHeader {
	char id[4];	  // チャンク毎のID
	int32_t size; // チャンクサイズ
};
// RIFFヘッダチャンク
struct RiffHeader {
	ChunkHeader chunk;	// "RIFF"
	char type[4];		// "WAVE"
};
// FMTチャンク
struct FormatChunk {
	ChunkHeader chunk; // "fmt"
	WAVEFORMATEX fmt;  // 波形フォーマット
};
// 音声データ
struct SoundData {
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;
};

class Audio
{
public:
	static Audio* GetInstance();

	// 初期化
	void Initialize(const SoundData& soundData);

	// 音の読み込み
	//void LoadSound(const SoundData& soundData, const char* filename);

//	// 再生
//	void Play();
//
//	// 停止
//	void Stop();
//
//private:
	// 解放処理
	void Finalize();

	// 音声データの読み込み
	SoundData SoundLoadWave(const char* filename);

	// 音声データ解放
	void SoundUnload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="soundData"></param>
	/// <param name="volume">音量</param>
	/// <param name="Semitones">音階</param>
	void SoundPlayWave(const SoundData& soundData, X3DAUDIO_VECTOR emitterPos, float volume, float Semitones);

	// 音声のループ再生
	void SoundPlayLoopingWave(const SoundData& soundData, float volume = 0.5f);

	// 
	void SoundPlayWithMuffledEffect(const SoundData& soundData, float volume, float Semitones);

	// XAPOの作成
	void CreateXAPO(IXAudio2SourceVoice* pSourceVoice);

private:
	IXAudio2MasteringVoice* masterVoice_;
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	X3DAUDIO_HANDLE X3DInstance_;
	IXAudio2SourceVoice* pSourceVoice_ = nullptr;
	IXAudio2SubmixVoice* pSubmixVoice_ = nullptr;
	//std::list<IXAudio2SourceVoice>* pSourceVoice_;
	//std::list<SoundData> soundData_;
};

