

DIR=Settings DDASEvent LendaEvent LendaPacker LendaFilter LendaBuilder TimmingAnalyzer2Channel TimmingAnalyzer4Channel

all:
	@for d in $(DIR); do make -j4 -C $$d  || exit; done
	@echo -e "\n\n ROOTLe Build Successful\n\n"
.PHONY: all

# all:
# 	-@cd ./Settings; make -j4
# 	-@cd ./DDASEvent; make -j4
# 	-@cd ./LendaEvent; make -j4
# 	-@cd ./LendaPacker; make -j4
# 	-@cd ./LendaFilter; make -j4
# 	-@cd ./LendaBuilder; make -j4

clean:
	@for d in $(DIR); do make clean -C $$d  || exit; done





