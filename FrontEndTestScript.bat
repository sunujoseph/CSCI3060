@echo off
SETLOCAL enabledelayedexpansion
cd /d %~dp0\test_documents\FrontEnd
set back=%cd%
for /d %%c in (*) do (
	cd %cd%\%%c\Input
	if not exist "%cd%\%%c\ActualOutput\" (mkdir "%cd%\%%c\ActualOutput")
	if not exist "%cd%\%%c\ActualDTF\" (mkdir "%cd%\%%c\ActualDTF")
	for %%f in (*) do (
		"%~dp0FrontEnd"\FrontEnd.exe "%cd%\current_user_accounts.txt" "%cd%\available items.txt" "%cd%\%%c\ActualDTF\%%f" > "%cd%\%%c\ActualOutput\%%f" < "%cd%\%%c\Input\%%f"
		set /A pass = 0
		fc /l "%cd%\%%c\ExpectedOutput\%%f" "%cd%\%%c\ActualOutput\%%f" > nul
		if errorlevel 1 (
			set /A pass = !pass! + 1
		)
		fc /l "%cd%\%%c\ExpectedDTF\%%f" "%cd%\%%c\ActualDTF\%%f" > nul
		if errorlevel 1 (
			set /A pass = !pass! + 2
		)
		if !pass! == 0 echo %%c: %%f Passed ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		if !pass! == 1 echo %%c: %%f Failed: Expected Output and Actual Output Don't Match
		if !pass! == 2 echo %%c: %%f Failed: Expected DTF and Actual DTF Don't Match
		if !pass! == 3 echo %%c: %%f Failed: Expected Output/DTF and Actual Output/DTF Don't Match 
	)
	cd %back%
)
ENDLOCAL