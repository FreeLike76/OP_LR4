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
	//=================================================================
	int32_t subchunk1Id;   // Завжди 0x666d7420 – літери "fmt "
	int32_t subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
	int32_t audioFormat;   // PCM = 1
	int16_t numChannels;   // Mono = 1, Stereo = 2
	int32_t sampleRate;    // Наприклад 44100
	int32_t byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
	int32_t blockAlign;    // == NumChannels * BitsPerSample/8
	int16_t bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
	//=================================================================
	int32_t subchunk2Id;   // 0x64617461 – літери "data"
	int32_t subchunk2Size; // == NumSamples * NumChannels * BitsPerSample/8, кількість байтів аудіоданих
};

class mWave
{
public:
	WavHead header;
	int16_t* data;
	mWave()
	{
		data = nullptr;
	};
	mWave(char* path)
	{
		FILE* input = fopen(path, "rb");
		fread(&header, sizeof(header), 1, input);
		data = new int16_t[header.subchunk2Size / header.blockAlign];

		for (int i = 0; i < header.subchunk2Size / header.blockAlign; i++)
		{
			fread(&data[i], header.blockAlign, 1, input);
		}
		fclose(input);
	}
	bool changeS(int coef)
	{
		if (coef <= 0 || data == nullptr)
			return false;
		int16_t* ndata = new int16_t[coef * header.subchunk2Size / header.blockAlign];
		for (int i = 0; i < header.subchunk2Size / header.blockAlign; i++)
		{
			for (int j = 0; j < coef; j++)
			{
				ndata[coef * i + j] = data[i];
			}
		}
		delete[] data;
		data = ndata;
		header.subchunk2Size *= coef;
		header.chunkSize = 36 + header.subchunk2Size;
		return true;
	}
	void saveTo(char* output)
	{
		ofstream fout;
		fout.open(output, ios::binary);
		fout << header.chunkId << header.chunkSize << header.format
			<< header.subchunk1Id << header.audioFormat << header.numChannels
			<< header.sampleRate << header.byteRate << header.blockAlign << header.bitsPerSample
			<< header.subchunk2Id << header.subchunk2Size;
		for (int i = 0; i < header.subchunk2Size / header.blockAlign; i++)
		{
			fout << data[i];
		}
		fout.close();
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
	int coef;
	cout << "Enter the path of input file (with .wav): ";
	cin >> in;
	input = stringtoArr(in);
	cout << "Enter the path of output file (with .wav): ";
	cin >> in;
	cout << "Enter the coeficient: ";
	cin >> coef;
	output = stringtoArr(in);
	mWave a(input);

}
