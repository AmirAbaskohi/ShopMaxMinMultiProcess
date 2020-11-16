all : fileDir ProvinceMain CityMain ShopMain FindMaxOrMin

fileDir:
	@mkdir -p build
	@mkdir -p bin
	@mkdir -p fifo

# Province
ProvinceMain : build/ProvinceMain.o build/Province.o build/Tools.o
			@echo "Making province executable file!"
			g++ build/ProvinceMain.o build/Province.o build/Tools.o -o bin/ProvinceMain

build/ProvinceMain.o : src/ProvinceMain.cpp src/Province.cpp src/Province.h
			g++ -c src/ProvinceMain.cpp -o build/ProvinceMain.o

build/Province.o : src/Province.cpp src/Province.h
			g++ -c src/Province.cpp -o build/Province.o

# City
CityMain : build/CityMain.o build/City.o build/Tools.o
			@echo "Making city executable file!"
			g++ build/CityMain.o build/City.o build/Tools.o -o bin/CityMain

build/CityMain.o : src/CityMain.cpp src/City.cpp src/City.h
			g++ -c src/CityMain.cpp -o build/CityMain.o

build/City.o : src/City.cpp src/City.h
			g++ -c src/City.cpp -o build/City.o

# Shop
ShopMain : build/ShopMain.o build/Shop.o build/Tools.o
			@echo "Making shop executable file!"
			g++ build/ShopMain.o build/Shop.o build/Tools.o -o bin/ShopMain

build/ShopMain.o : src/ShopMain.cpp src/Shop.cpp src/Shop.h
			g++ -c src/ShopMain.cpp -o build/ShopMain.o

build/Shop.o : src/Shop.cpp src/Shop.h
			g++ -c src/Shop.cpp -o build/Shop.o

# FindMaxOrMin
FindMaxOrMin : build/Main.o build/FindMaxOrMin.o build/Tools.o
			@echo "Linking ..."
			g++ build/Main.o build/FindMaxOrMin.o build/Tools.o -o bin/FindMaxOrMin

build/Main.o : src/Main.cpp src/FindMaxOrMin.h src/FindMaxOrMin.cpp
			g++ -c src/Main.cpp -o build/Main.o

build/Tools.o : src/Tools.cpp src/Tools.h
			g++ -c src/Tools.cpp -o build/Tools.o

build/FindMaxOrMin.o : src/FindMaxOrMin.cpp src/FindMaxOrMin.h
			g++ -c src/FindMaxOrMin.cpp -o build/FindMaxOrMin.o

clean:
	rm -f -r build
	rm -f -r bin
	rm -f -r fifo
	rm -f FindMaxOrMin