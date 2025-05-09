		 BUILD   := build
override OBJD    := $(BUILD)/obj
		 SRCD    := src
		 EXENAME := diffure
override EXE     := $(BUILD)/$(EXENAME)

		 SRC     := $(wildcard $(SRCD)/*.c)
override OBJ     := $(patsubst $(SRCD)/%.c,$(OBJD)/%.o,$(SRC))
override TESTOBJ := $(patsubst $(OBJ)/%.o,$(OBJD)/%.gcno,$(OBJ))

# 		 TSRC = $(wildcard $(SRCD)/test/*.c)
# override TOBJ = $(patsubst $(SRCD)/test/%.c,$(OBJD)/test/%.o,$(TSRC))

SETTINGS    =

CC         ?= gcc
LD         ?= gcc
CFLAGS     += -Wall -Wpedantic -Wconversion -fPIE -pie
LDFLAGS    +=
ASAN_FLAGS += -fsanitize=undefined,null,alignment,builtin -fstack-check -fstrict-flex-arrays=3
ASANCC_FLAGS += $(ASAN_FLAGS) -fstack-protector-strong
ASANLD_FLAGS += $(ASAN_FLAGS) -fstack-clash-protection -dead-strip
INST       ?= gcov
INSTFLAGS  ?= -n

ENVVARS    +=
LIBS       += 

PACK       += strip
PACKFLAGS  += -o $(EXE)



.PHONY: all
all: ## Release build
all: CFLAGS += $(ASANCC_FLAGS)
all: LDFLAGS += $(ASANLD_FLAGS)
all: tiny

tiny: CFLAGS += -Os -DNDEBUG -ffunction-sections -fdata-sections -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 
tiny: LDFLAGS += -ffunction-sections -fdata-sections -U_FORTIFY_SOURCE -D_FORTIFY_SOURCE=3 
tiny: pure pack

test: ## Debug build and test instrumentation
test: CFLAGS  += $(ASANCC_FLAGS) --coverage -pg -fprofile-arcs -ftest-coverage
test: LDFLAGS += $(ASANLD_FLAGS) --coverage
#test: SRC += $(TSRC)
#test: OBJ += $(TOBJ)
#test: CFLAGS += -finstrument-functions
#test: LDFLAGS += -finstrument-functions
test: debug
	@touch $(BUILD)/testfile $(BUILD)/testfile2
	$(EXE) -v -e echo -d 'echo meow mrp' -p $(BUILD)/testfile
	@echo "--------------- RUN1  DONE ---------------"
	$(EXE) -v -e echo -d 'echo meow mrp' -o $(BUILD)/testfile2.patch $(BUILD)/testfile $(BUILD)/testfile2
	@echo "--------------- RUN2  DONE ---------------"
	$(EXE) -v -e echo -d 'echo meow mrp' -o $(BUILD)/testfile3/ $(BUILD)/testfile $(BUILD)/testfile2
	@echo "--------------- RUN3  DONE ---------------"
	$(INST) $(INSTFLAGS) $(TESTOBJ)
	@echo "--------------- TEST DONE ---------------"

debug: ## Debug build
debug: CFLAGS  += -g2
debug: LDFLAGS += -g2
debug: pure

pure: setup $(OBJ) $(EXE)
	@echo "--------------- BUILD DONE ---------------"

.NOTPARALLEL:
#	 clean setup pack dry help test

pack: ## Strip and pack executable
	$(PACK) $(EXE) $(PACKFLAGS)
	@echo "-------------- PACKING DONE --------------"

setup: clean ## Setup build env
	@mkdir -p $(OBJD)
	@echo "--------------- SETUP DONE ---------------"

clean: ## Clean built env
	@rm -rf $(BUILD)
	@echo "--------------- CLEAN DONE ---------------"

dry: ## Dry run and show paths
	@echo $(CC) $(LD)\\n$(EXE)\\n  ^- $(OBJ)\\n ^^- $(SRC)\\ntests\\n ^- $(TESTOBJ)

.PHONY: help
help:  ## Display this help
	@awk 'BEGIN {FS = ":.*##"; printf "Usage:\n  make \033[36m\033[0m\n"} /^[$$()% a-zA-Z_-]+:.*?##/ \
	{ printf "    \033[36m%-15s\033[0m %s\n", $$1, $$2 } /^##@/ { printf "\n\033[1m%s\033[0m\n", substr($$0, 5) } ' $(MAKEFILE_LIST)

### Main building script

$(EXE): $(OBJ)
	$(ENVVARS) $(CC) $(LDFLAGS) $^ $(LIBS) $(SETTINGS) -o $@

$(OBJD)/%.o: $(SRCD)/%.c
	$(ENVVARS) $(CC) $(CFLAGS) -c $< $(SETTINGS) -o $@

