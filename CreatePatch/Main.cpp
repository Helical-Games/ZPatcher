// CreatePatch.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Lzma2Encoder.h"
#include "Lzma2Decoder.h"
#include "FileUtils.h"
#include "CreatePatch.h"
#include "LogSystem.h"

// Check windows
#if _WIN32 || _WIN64
	#if _WIN64
		#define ENVBITS 64
	#else
		#define ENVBITS 32
	#endif
#endif

// Check GCC
#if __GNUC__
	#if __x86_64__ || __ppc64__
		#define ENVBITS 64
	#else
		#define ENVBITS 32
	#endif
#endif

int main(int argc, char* argv[])
{
	using namespace ZPatcher;

	fprintf(stderr, "\nCreatePatch : ZPatcher [%d] v1.0 beta : %s\n", ENVBITS, __DATE__);
	fprintf(stderr, "Copyright (c) 2016 Felipe \"Zoc\" Silveira : http://www.github.com/TheZoc/ZPatcher\n\n");

	if (argc < 4)
	{
		fprintf(stderr, "Usage: %s <old version directory> <new version directory> <output patch file>\n", argv[0]);
		exit(EXIT_SUCCESS);
	}

	InitLogSystem("./");

	// Store our targets
	std::string oldDirectory = argv[1];
	std::string newDirectory = argv[2];
	std::string outputFilename = argv[3];

	// Make sure all directories are represented in the same format
	NormalizeFileName(oldDirectory);
	NormalizeFileName(newDirectory);
	NormalizeFileName(outputFilename);

	Log(LOG, "Output patch file: %s", outputFilename.c_str());
	Log(LOG, "Old version directory: %s", oldDirectory.c_str());
	Log(LOG, "New version directory: %s", newDirectory.c_str());

	PatchFileList_t* patchFileList = GetDifferences(oldDirectory, newDirectory);

	CreatePatchFile(outputFilename, newDirectory, patchFileList);

	if (patchFileList)
		delete(patchFileList);

	DestroyLogSystem();

//	system("pause");

	exit(EXIT_SUCCESS);

}


//////////////////////////////////////////////////////////////////////////
// Main function 


/*
int main()
{
std::vector<std::string> fileList;

ZPatcher::GetFilesInDirectory(fileList, ".");

for (std::vector<std::string>::iterator itr = fileList.begin(); itr < fileList.end(); ++itr)
{
fprintf(stderr, "%s\n", itr->c_str());
}

system("pause");
}
*/


/*
int main()
{
using namespace ZPatcher;

FILE* sourceFile;
FILE* destFile;

errno_t err = 0;

char sourceName[] = _T("a.jpg");
char compressedName[] = _T("test.zoc");
char destName[] = _T("b.jpg");

// 	char sourceName[] = _T("a.txt");
// 	char compressedName[] = _T("txt.zoc");
// 	char destName[] = _T("b.txt");

err = fopen_s(&sourceFile, sourceName, _T("rb"));
assert(err == 0);
err = fopen_s(&destFile, compressedName, _T("wb"));
assert(err == 0);

CLzma2EncHandle hLzma2Enc = InitLzma2Encoder();

{
// This makes props a local variable
Byte props = Lzma2Enc_WriteProperties(hLzma2Enc);
WritePatchFileHeader(destFile, props);
}

WriteFileInfo(destFile, destName);
FileCompress(hLzma2Enc, sourceFile, destFile);
DestroyLzma2EncHandle(hLzma2Enc);

fclose(sourceFile);
fclose(destFile);

fprintf(stderr, "\nFile compression completed.\n");

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

err = fopen_s(&sourceFile, compressedName, _T("rb"));
assert(err == 0);
err = fopen_s(&destFile, destName, _T("wb"));
assert(err == 0);

Byte props;
if (!ReadPatchFileHeader(sourceFile, props))
{
fprintf(stderr, "\nFile decompression failed - Invalid header.\n");
system("pause");
return 1;
}

CLzma2Dec* decoder = InitLzma2Decoder(props);

std::wstring outFileName;
GetFileinfo(sourceFile, outFileName);

fprintf(stderr, "\nDecompressing file %ls.\n", outFileName.c_str());

FileDecompress(decoder, sourceFile, destFile);

fclose(sourceFile);
fclose(destFile);

fprintf(stderr, "\nFile decompression completed.\n");

system("pause");

return 0;
}

//*/
