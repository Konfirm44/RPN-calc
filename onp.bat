@ECHO OFF
konp.exe -i pliki\data.txt && GOTO :SUC
echo.
echo ERROR OCCURED.
GOTO :END
:SUC
echo.
echo Process returned 0.
echo.
REM type pliki\output.txt
echo.
:END