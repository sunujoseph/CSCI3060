@echo off
SETLOCAL enabledelayedexpansion
cd /d %~dp0\FrontEnd
FrontEnd.exe > "%~dp0test.txt" < "%~dp0test_documents\FrontEnd\Login\Input\get_user_test.txt"
Rem FrontEnd.exe < "%~dp0test_documents\FrontEnd\Login\Input\get_user_test.txt" > "%~dp0test.txt"
Rem TYPE "%~dp0test_documents\FrontEnd\Login\Input\get_user_test.txt"
ENDLOCAL