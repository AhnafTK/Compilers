:: COMPILERS COURSE - SCRIPT ---------------------------------------------
:: SCRIPT A22 - CST8152 - Winter 2023

CLS
SET COMPILER=ZeusCompiler.exe

SET FILE0=INPUT0_Empty
SET FILE1=INPUT1_Hello
SET FILE2=INPUT2_Keyword
SET FILE3=INPUT3_Factorial
SET FILE4=INPUT4_Datatypes
SET FILE5=INPUT5_Err1Reader
SET FILE6=INPUT6_Conditional
SET FILE7=INPUT7_Zscore
SET FILE8=INPUT8_Employee
SET FILE9=INPUT9_Big
SET FILE10=INPUT10_Calculator
SET FILE11=INPUT11_WeightPlateCalc

SET ASSIGNMENT=A22
SET EXTENSION=zs
SET OUTPUT=out
SET ERROR=err

SET PARAM=S

:: ---------------------------------------------------------------------
:: Begin of Tests (A22 - W22) ------------------------------------------
:: ---------------------------------------------------------------------

@echo off

ECHO "                                                                       "
ECHO "                                                                       "
ECHO "[SCANNER SCRIPT .......................................................]"
ECHO "                                                                       "

ren *.exe %COMPILER%

::
:: BASIC TESTS  ----------------------------------------------------------
::
:: Basic Tests (A22 - W22) - - - - - - - - - - - - - - - - - - - - - -

%COMPILER% %PARAM% %FILE0%.%EXTENSION%	> %FILE0%-%ASSIGNMENT%.%OUTPUT%	2> %FILE0%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE1%.%EXTENSION%	> %FILE1%-%ASSIGNMENT%.%OUTPUT%	2> %FILE1%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE2%.%EXTENSION%	> %FILE2%-%ASSIGNMENT%.%OUTPUT%	2> %FILE2%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE3%.%EXTENSION%	> %FILE3%-%ASSIGNMENT%.%OUTPUT%	2> %FILE3%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE4%.%EXTENSION%	> %FILE4%-%ASSIGNMENT%.%OUTPUT%	2> %FILE4%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE5%.%EXTENSION%	> %FILE5%-%ASSIGNMENT%.%OUTPUT%	2> %FILE5%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE6%.%EXTENSION%	> %FILE6%-%ASSIGNMENT%.%OUTPUT%	2> %FILE6%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE7%.%EXTENSION%	> %FILE7%-%ASSIGNMENT%.%OUTPUT%	2> %FILE7%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE8%.%EXTENSION%	> %FILE8%-%ASSIGNMENT%.%OUTPUT%	2> %FILE8%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE9%.%EXTENSION%	> %FILE9%-%ASSIGNMENT%.%OUTPUT%	2> %FILE9%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE10%.%EXTENSION%	> %FILE10%-%ASSIGNMENT%.%OUTPUT%	2> %FILE10%-%ASSIGNMENT%.%ERROR%
%COMPILER% %PARAM% %FILE11%.%EXTENSION%	> %FILE11%-%ASSIGNMENT%.%OUTPUT%	2> %FILE11%-%ASSIGNMENT%.%ERROR%

:: SHOW OUTPUTS - - - - - - - - - - - - - - - - - - - - - - - - - - -
DIR *.OUT
DIR *.ERR

:: ---------------------------------------------------------------------
:: End of Tests (A12 - W22) --------------------------------------------
:: ---------------------------------------------------------------------
