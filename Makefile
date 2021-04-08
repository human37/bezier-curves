run:
	g++ *.cpp -o bezier.exe -std=c++11 -lGL -lGLU -lglut
	./bezier.exe

clean:
	rm -rf *.exe *.o