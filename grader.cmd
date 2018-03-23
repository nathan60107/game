E:
cd E:\SystemFile\Desktop\CODING\C++\CompetitiveProgrammingTraining\ioi2013\game\game
PAUSE
for %%i in (*) do (
start "game" "E:\SystemFile\Desktop\CODING\C++\CompetitiveProgrammingTraining\ioi2013\game\bin\Debug\game.exe" "%%~ni"
)
PAUSE
for %%i in (*) do (
start "winmerge" "E:\SystemFile\Desktop\¶w∏À¿…≠Ã\WinMerge-2.14.0-exe\winmergeu.exe" /f "Exclude Source Control" "out\%%~ni.myout" "out\%%~ni.out"
)