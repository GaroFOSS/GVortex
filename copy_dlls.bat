@echo off
echo --- Copying DLLs for GTK Application ---

:: Create a distribution folder
set DIST_FOLDER=dist
if not exist "%DIST_FOLDER%" mkdir "%DIST_FOLDER%"

:: Copy your main executable and assets
echo Copying application files...
copy downloader.exe "%DIST_FOLDER%\" > nul
copy style.css "%DIST_FOLDER%\" > nul
copy button_background.png "%DIST_FOLDER%\" > nul


:: The location of your MSYS2 MinGW64 binary files
set MINGW_BIN=C:\msys64\mingw64\bin

:: List of essential DLLs to copy
set DLL_LIST=^
libatk-1.0-0.dll ^
libbrotlidec.dll ^
libbz2-1.dll ^
libcairo-2.dll ^
libcairo-gobject-2.dll ^
libepoxy-0.dll ^
libexpat-1.dll ^
libffi-8.dll ^
libfontconfig-1.dll ^
libfreetype-6.dll ^
libfribidi-0.dll ^
libgcc_s_seh-1.dll ^
libgdk-3-0.dll ^
libgdk_pixbuf-2.0-0.dll ^
libgio-2.0-0.dll ^
libglib-2.0-0.dll ^
libgmodule-2.0-0.dll ^
libgobject-2.0-0.dll ^
libgraphite2-0.dll ^
libgtk-3-0.dll ^
libharfbuzz-0.dll ^
libiconv-2.dll ^
libintl-8.dll ^
libjpeg-8.dll ^
libpango-1.0-0.dll ^
libpangocairo-1.0-0.dll ^
libpangoft2-1.0-0.dll ^
libpangowin32-1.0-0.dll ^
libpcre-1.dll ^
libpcre2-8-0.dll ^
libpixman-1-0.dll ^
libpng16-16.dll ^
libstdc++-6.dll ^
libthai-0.dll ^
libtiff-6.dll ^
libwinpthread-1.dll ^
zlib1.dll

:: Loop through the list and copy each DLL
echo.
echo --- Copying required system DLLs ---
for %%f in (%DLL_LIST%) do (
    echo Copying %%f...
    copy "%MINGW_BIN%\%%f" "%DIST_FOLDER%\" > nul
)

echo.
echo --- Copying GTK Settings and Themes ---

:: GTK needs its settings and themes to look correct
xcopy /E /I /Y "C:\msys64\mingw64\share\glib-2.0" "%DIST_FOLDER%\share\glib-2.0" > nul
xcopy /E /I /Y "C:\msys64\mingw64\share\gtk-3.0" "%DIST_FOLDER%\share\gtk-3.0" > nul
xcopy /E /I /Y "C:\msys64\mingw64\lib\gdk-pixbuf-2.0" "%DIST_FOLDER%\lib\gdk-pixbuf-2.0" > nul

echo.
echo --- Adding Dependencies ---
echo Remember to add yt-dlp.exe and ffmpeg.exe to the '%DIST_FOLDER%' directory!
echo.
echo --- Distribution package created in '%DIST_FOLDER%' folder. ---
pause
