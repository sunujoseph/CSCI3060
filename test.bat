@echo off
SETLOCAL enabledelayedexpansion
cd /d %~dp0\test_documents\FrontEnd
echo %cd%\available_items.txt
fc /b "%cd%\available items.txt" "%cd%\current_user_accounts.txt"