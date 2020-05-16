// OP LR4
// IP91 Dmytro Geleshko
// Var 2
#include <iostream>
#include <vector>
#include <stdio.h>
#include <fstream>

using namespace std;

struct WavHead
{
	int32_t chunkId;   // Завжди містить значення 0x52494646 (літери "RIFF")
	int32_t chunkSize; // 36 + розмір другого підрозділу в байтах
					   // Іншими словами 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
					   // Це розмір всього файла мінус 8 байтів які займають поля chunkId та chunkSize
	int32_t format;    // Для wav-файла це завжди 0x57415645 (літери "WAVE")
	int32_t subchunk1Id;   // Завжди 0x666d7420 – літери "fmt "
	int32_t subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
	int32_t audioFormat;   // PCM = 1
	int16_t numChannels;   // Mono = 1, Stereo = 2
	int32_t sampleRate;    // Наприклад 44100
	int32_t byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
	int32_t blockAlign;    // == NumChannels * BitsPerSample/8
	int16_t bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
	int32_t subchunk1Id;   // Завжди 0x666d7420 – літери "fmt "
	int32_t subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
	int32_t audioFormat;   // PCM = 1
	int16_t numChannels;   // Mono = 1, Stereo = 2
	int32_t sampleRate;    // Наприклад 44100
	int32_t byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
	int32_t blockAlign;    // == NumChannels * BitsPerSample/8
	int16_t bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.

};

class mWave
{
	WavHead inf;
	int16_t* data;
	mWave(char* path)
	{
		FILE* input = fopen(path, "rb");
		fread(&inf, sizeof(inf), 1, input);
	
	}

};

char* stringtoArr(string in)
{
	char* res = new char[in.size()+1];
	for (int i = 0; i < in.size(); i++)
	{
		res[i] = in[i];
	}
	res[in.size()] = '\0';
	return res;
}

int main()
{
	string in;
	char* input, * output;
	cout << "Enter the path of input file (with .wav): ";
	cin >> in;
	input = stringtoArr(in);
	cout << "Enter the path of output file (with .wav): ";
	cin >> in;
	output = stringtoArr(in);
	mWave a(input);

}
