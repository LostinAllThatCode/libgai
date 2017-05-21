#ifndef _INCLUDE_GAI_PLATFORM_WIN_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

PLATFORM_API_READ_FILE(gaipfwin_ReadFile)
{
	HANDLE file_handle;
	file_handle = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, 0,
	                         OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (!file_handle) return 0;

	LARGE_INTEGER file_size;
	DWORD bytes_read;
	GetFileSizeEx(file_handle, &file_size);
	file->data = VirtualAlloc(0, file_size.QuadPart, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	ReadFile(file_handle, file->data, file_size.QuadPart - 1, &bytes_read, 0);
	if (bytes_read != (file_size.QuadPart - 1))
	{
		CloseHandle(file_handle);
		VirtualFree(file->data, 0, MEM_RELEASE);
		file->data = 0;
	    return 0;
	}
	file->size = bytes_read;
	CloseHandle(file_handle);
	return 1;
}


PLATFORM_API_LOAD_BITMAP(gaipfwin_LoadBitmapFromFile)
{
	if (!bitmap) assert(!"error");
	int bpp;
	stbi_set_flip_vertically_on_load(0);
	bitmap->memory 		= stbi_load(filename, &bitmap->width, &bitmap->height, &bpp, 4);
	bitmap->is_loaded 	= 0;
	bitmap->handle 		= 0;
}

PLATFORM_API_LOAD_FONT(gaipfwin_LoadFontFromFile)
{
	gaipf_file file;
	if(!gaipfwin_ReadFile(&file, filename)) return;
	
	stbtt_pack_context font_context = font->stbtt_context;
	stbtt_packedchar   *characters  = font->stbtt_chars;

	font->bitmap.is_loaded 			= 0;
	font->bitmap.width 				= 4096;
	font->bitmap.height 			= 4096;
	font->bitmap.handle            	= 0;
	font->bitmap.is_font_texture   	= 1;

	// Create temporary bitmap buffer for the texture
	font->bitmap.memory = (unsigned char *) VirtualAlloc(0, font->bitmap.width * font->bitmap.height, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE );

	stbtt_PackBegin(&font_context, font->bitmap.memory, font->bitmap.width, font->bitmap.height, 0, 1, 0);
	stbtt_PackSetOversampling(&font_context, 2, 2);
	stbtt_PackFontRange(&font_context, (u08 *)file.data, 0, size, 32, 1024 - 32, characters);
	stbtt_PackEnd(&font_context);

	VirtualFree(file.data, 0, MEM_RELEASE);
}

#define _INCLUDE_GAI_PLATFORM_WIN_H
#endif