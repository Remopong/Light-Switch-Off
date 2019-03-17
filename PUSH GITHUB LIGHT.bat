@Echo OFF
cd D:\Users\Remi\Desktop\Arduino_Light
set /p commitName="Nom du commit (pas d'espaces): "
@echo %commitName%
Git add *
Git commit -m %commitName%
Git push origin master