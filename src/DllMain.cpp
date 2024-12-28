#include <Windows.h>

#include "nethost.h"
#include "plugin_sa.h"
#include "CFont.h"
#include "CRadar.h"

const char* g_DotNetLogo = ".NET 6.0";
void* g_DotNetLibrary = nullptr;

bool Initialize();

BOOL APIENTRY DllMain(HINSTANCE hInstance, INT fwReason, LPVOID lpvReserved)
{
	static volatile char initialized = 0;

	if (initialized && fwReason == DLL_PROCESS_DETACH)
	{
		if (g_DotNetLibrary) {
			FreeLibrary(static_cast<HMODULE>(g_DotNetLibrary));
			g_DotNetLibrary = nullptr;
		}
		return 1;
	}
	else if (fwReason == DLL_PROCESS_ATTACH && !initialized) {
		initialized = 1;
		return Initialize();
	}

	return 0;
}

int MsgBox(int style, const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	int nb = vsnprintf(0, 0, format, ap);
	char* buf = (char*)malloc(nb + 1);
	vsnprintf(buf, nb, format, ap);
	va_end(ap);

	int result = MessageBoxA(0, buf, 0, style);
	free(buf);

	return result;
}

bool Initialize()
{
	char_t buf[MAX_PATH];
	size_t len = sizeof(buf) / sizeof(char_t);

	if (get_hostfxr_path(buf, &len, nullptr)) {
		MsgBox(MB_OK | MB_ICONWARNING, "Unable to find dotnet library!");
		return 0;
	}

	g_DotNetLibrary = LoadLibraryW(buf);

	if (!g_DotNetLibrary) {
		MsgBox(MB_OK | MB_ICONERROR, "Unable to load dotnet library. (code: %d)", GetLastError());
		return 0;
	}

	plugin::Events::drawingEvent += []
		{
			CFont::SetOrientation(ALIGN_LEFT);
			CFont::SetColor(CRGBA(200, 200, 200, 255));
			CFont::SetScale(1, 1.5);
			CFont::SetFontStyle(FONT_PRICEDOWN);

			CFont::PrintString(10, 10, g_DotNetLogo);
		};

	return 1;
}