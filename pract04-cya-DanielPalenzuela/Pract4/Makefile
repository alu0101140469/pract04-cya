CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic

TARGET = p04_html_analyzer

OBJECTS = p04_html_analyzer.o \
          html_document.o \
          html_analyzer.o \
          report_generator.o

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

p04_html_analyzer.o: p04_html_analyzer.cc \
                     html_analyzer.h \
                     html_document.h \
                     report_generator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

html_document.o: html_document.cc html_document.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

html_analyzer.o: html_analyzer.cc \
                 html_analyzer.h \
                 html_document.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

report_generator.o: report_generator.cc \
                    report_generator.h \
                    html_document.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJECTS)