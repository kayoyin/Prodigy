all: translate translateback

translate: translate
	g++ -std=c++11 translate.cpp -o translate

translateback: translateback
	g++ -std=c++11 translatebackv.cpp -o translateback

clean: translate translateback
	rm translate translateback
