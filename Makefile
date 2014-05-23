

DIR=Settings DDASEvent LendaEvent LendaPacker LendaFilter LendaBuilder TimmingAnalyzer2Channel TimmingAnalyzer4Channel

all:
	@for d in $(DIR); do make -j4 -C $$d  || exit; done
	@echo -e "\n\n ROOTLe Build Successful\n\n"
.PHONY: all

clean:
	@for d in $(DIR); do make clean -C $$d  || exit; done





