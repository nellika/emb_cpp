##
## header-start
##################################################################################
##
## \file      Makefile
##
## \brief     This file belongs to the C++ tutorial project
##
## \author    Nelli Nyisztor
##
##################################################################################
## header-end
##

LDFLAGS            :=
CC                 := g++
OBJDIR             := ./build

CFLAGS_COMMON      := -Wall -Wshadow -Wextra -Wno-unused-parameter -Werror
CFLAGS_COMMON      += -std=c++14

CFLAGS_DEBUG       := $(CFLAGS_COMMON) -O -g
CFLAGS             := $(CFLAGS_COMMON) -O3


TARGET             := map2
SRC_FILES          := \
  ./map2.cpp


OBJECT_FILES       := $(addprefix  $(OBJDIR)/,$(notdir $(SRC_FILES:.cpp=.r.o)))
OBJECT_FILES_DEBUG := $(addprefix  $(OBJDIR)/,$(notdir $(SRC_FILES:.cpp=.d.o)))
DEP_FILES          := $(addprefix  $(OBJDIR)/,$(notdir $(SRC_FILES:.cpp=.r.P)))
DEP_FILES_DEBUG    := $(addprefix  $(OBJDIR)/,$(notdir $(SRC_FILES:.cpp=.d.P)))


$(TARGET): $(OBJECT_FILES) Makefile
	$(CC) -o $@ $(OBJECT_FILES) $(LDFLAGS)

debug: $(OBJECT_FILES_DEBUG) Makefile
	$(CC) -o $(TARGET).$@ $(OBJECT_FILES_DEBUG) $(LDFLAGS)



$(OBJECT_FILES)        :| $(OBJDIR)
$(OBJECT_FILES_DEBUG)  :| $(OBJDIR)

#
# Generation of dependency include files during compilation
# which will be used during the next compilation
# thanks to  http://make.paulandlesley.org/autodep.html
#


$(OBJDIR)/%.r.o : ./%.cpp
	@printf "Compiling release version of $<\n"
	$(CC) -c -MMD -MF $(OBJDIR)/$*.r.d -MT $(OBJDIR)/$*.r.o $(CFLAGS) -o $@ $<
	@cp $(OBJDIR)/$*.r.d $(OBJDIR)/$*.r.P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(OBJDIR)/$*.r.d >> $(OBJDIR)/$*.r.P
	@rm -f $(OBJDIR)/$*.r.d

$(OBJDIR)/%.d.o : ./%.cpp
	@printf "Compiling debug version of $<\n"
	$(CC) -c -MMD -MF $(OBJDIR)/$*.d.d -MT $(OBJDIR)/$*.d.o $(CFLAGS_DEBUG) -o $@ $<
	@cp $(OBJDIR)/$*.d.d $(OBJDIR)/$*.d.P
	@sed -e 's/#.*//' -e 's/^[^:]*: *//' -e 's/ *\\$$//' \
	-e '/^$$/ d' -e 's/$$/ :/' < $(OBJDIR)/$*.d.d >> $(OBJDIR)/$*.d.P
	@rm -f $(OBJDIR)/$*.d.d


$(OBJDIR):
	mkdir -p $@


clean:
	$(RM) $(TARGET) $(TARGET).debug $(OBJECT_FILES)  $(OBJECT_FILES_DEBUG) $(DEP_FILES) $(DEP_FILES_DEBUG)


.SUFFIXES:              # Delete the default suffixes
.SUFFIXES: .cpp .o .h   # Define "CPP" language suffix list

-include $(DEP_FILES)
-include $(DEP_FILES_DEBUG)




## NOTE#1
## target: | dir
## test for existence dependency, not for time dependency
##
## NOTE#2
## additional stuff
## generation of assembly code GAS syntax
##
## g++ -march=core-avx2 -Wall -O3 -std=c++11 -S -fverbose-asm -Wa,-adhln main.cpp
##
