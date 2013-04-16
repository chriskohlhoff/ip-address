ifndef CXX
CXX = g++-4.7
endif

ifndef CXXFLAGS
CXXFLAGS = -std=c++11 -O2 -Wall -Wextra
endif

TESTS = \
	network \
  ip/address \
  ip/address_v4 \
  ip/address_v6

OBJFILES = $(TESTS:%=%.o)

all: $(TESTS)

clean:
	rm -f $(OBJFILES)

clobber:
	rm -f $(OBJFILES) $(TESTS)

$(TESTS): %: %.o
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJFILES): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -I../include -o $@ -c $<
