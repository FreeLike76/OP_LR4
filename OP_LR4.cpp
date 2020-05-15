// OP LR4
// IP91 Dmytro Geleshko
// Var 2
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

class mWave
{
public:
	//RIFFHEADRE
	int32_t chunkId;   // Завжди містить значення 0x52494646 (літери "RIFF")
	int32_t chunkSize; // 36 + розмір другого підрозділу в байтах
					   // Іншими словами 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
					   // Це розмір всього файла мінус 8 байтів які займають поля chunkId та chunkSize
	int32_t format;    // Для wav-файла це завжди 0x57415645 (літери "WAVE")
	//SUBCHUNK1
	int32_t subchunk1Id;   // Завжди 0x666d7420 – літери "fmt "
	int32_t subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
	int32_t audioFormat;   // PCM = 1
	int16_t numChannels;   // Mono = 1, Stereo = 2
	int32_t sampleRate;    // Наприклад 44100
	int32_t byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
	int32_t blockAlign;    // == NumChannels * BitsPerSample/8
	int16_t bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
	//SUBCHUNK2
	int32_t subchunk2Id;   // 0x64617461 – літери "data"
	int32_t subchunk2Size; // == NumSamples * NumChannels * BitsPerSample/8, кількість байтів аудіоданих
	int16_t* data;         // семпли

	mWave() { data = nullptr; };
	mWave(char* path)
	{
		FILE* input;
		fopen_s(&input, path, "rb");
		fread(&chunkId, sizeof(chunkId), 1, input);
		fread(&chunkSize, sizeof(chunkSize), 1, input);
		fread(&format, sizeof(format), 1, input);
		fread(&subchunk1Id, sizeof(subchunk1Id), 1, input);
		fread(&subchunk1Size, sizeof(subchunk1Size), 1, input);
		fread(&audioFormat, sizeof(audioFormat), 1, input);
		fread(&numChannels, sizeof(numChannels), 1, input);
		fread(&sampleRate, sizeof(sampleRate), 1, input);
		fread(&byteRate, sizeof(byteRate), 1, input);
		fread(&blockAlign, sizeof(blockAlign), 1, input);
		fread(&bitsPerSample, sizeof(bitsPerSample), 1, input);
		fread(&subchunk2Id, sizeof(subchunk2Id), 1, input);
		fread(&subchunk2Size, sizeof(subchunk2Size), 1, input);

		data = new int16_t[subchunk2Size / blockAlign];
	}
};




int main()
{



}
