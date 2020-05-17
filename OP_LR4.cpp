#define _CRT_SECURE_NO_WARNINGS
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
	char chunkId[4];   // Завжди містить значення 0x52494646 (літери "RIFF")
	unsigned long chunkSize; // 36 + розмір другого підрозділу в байтах
					   // Іншими словами 4 + (8 + SubChunk1Size) + (8 + SubChunk2Size)
					   // Це розмір всього файла мінус 8 байтів які займають поля chunkId та chunkSize
	char format[4];    // Для wav-файла це завжди 0x57415645 (літери "WAVE")
	//=================================================================
	char subchunk1Id[4];   // Завжди 0x666d7420 – літери "fmt "
	unsigned long subchunk1Size; // Завжди 16 для аудіо PCM. Це розмір частини підрозділу, що слідує після цього числа
	unsigned short audioFormat;   // PCM = 1
	unsigned short numChannels;   // Mono = 1, Stereo = 2
	unsigned long sampleRate;    // Наприклад 44100
	unsigned long byteRate;      // == SampleRate * NumChannels * BitsPerSample/8
	unsigned short blockAlign;    // == NumChannels * BitsPerSample/8
	unsigned short bitsPerSample; // 8 bits = 8, 16 bits = 16, etc.
};
struct WavDataChunk
{
	char subchunk2Id[4];   // 0x64617461 – літери "data"
	unsigned long subchunk2Size; // == NumSamples * NumChannels * BitsPerSample/8, кількість байтів аудіоданих
};

class mWave
{
public:
	WavHead header;
	WavDataChunk dataChunk;
	short int* data;
	mWave()
	{
		data = nullptr;
	};
	mWave(char* path)
	{
		FILE* input = fopen(path, "rb");
		fread(&header, sizeof(header), 1, input);

		while (true)
		{
			fread(&dataChunk, sizeof(dataChunk), 1, input);
			if (*(unsigned int*)&dataChunk.subchunk2Id == 0x61746164)
				break;
			//skip chunk data bytes
			fseek(input, dataChunk.subchunk2Size, SEEK_CUR);
		}

		data = new short int[dataChunk.subchunk2Size / header.blockAlign];

		for (int i = 0; i < dataChunk.subchunk2Size / header.blockAlign; i++)
		{
			fread(&data[i], header.blockAlign, 1, input);
		}
		fclose(input);
	}
	~mWave()
	{
		if (data != nullptr)
			delete[] data;
	}
	bool changeS(int coef)
	{
		if (coef <= 0 || data == nullptr)
			return false;
		short int* ndata = new short int[coef * dataChunk.subchunk2Size / header.blockAlign];
		for (int i = 0; i < dataChunk.subchunk2Size / header.blockAlign; i++)
		{
			for (int j = 0; j < coef; j++)
			{
				ndata[coef * i + j] = data[i];
			}
		}
		delete[] data;
		data = ndata;
		dataChunk.subchunk2Size *= coef;
		header.chunkSize = 36 + dataChunk.subchunk2Size;
		return true;
	}
	void saveTo(char* output)
	{
		ofstream create;
		create.open(output, ios::binary);
		create.close();
		FILE* fout = fopen(output, "w");
		fwrite(&header, sizeof(header), 1, fout);
		fwrite(&dataChunk, sizeof(dataChunk), 1, fout);
		for (int i = 0; i < dataChunk.subchunk2Size / header.blockAlign; i++)
		{
			fwrite(&data[i], header.blockAlign, 1, fout);
		}
		fclose(fout);
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
	a.changeS(coef);
	a.saveTo(output);

}
