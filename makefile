######################################
# makefile for programs and generators
######################################

CCOMP = cc
CFLAGS = -O4

all: progs gens

# problem generators
gens: bin/spacyc bin/spgrid bin/sprand

bin/spacyc: src/spacyc.c src/random.c
	$(CCOMP) $(CFLAGS) -o bin/spacyc src/spacyc.c

bin/spgrid: src/spgrid.c src/random.c
	$(CCOMP) $(CFLAGS) -o bin/spgrid src/spgrid.c

bin/sprand: src/sprand.c src/random.c
	$(CCOMP) $(CFLAGS) -o bin/sprand src/sprand.c

# shortest paths programs
progs: bin/acc bin/bf bin/bfp bin/dikb bin/dikba bin/dikbd bin/dikbm bin/dikh\
bin/dikr bin/dikf bin/dikq bin/gor bin/gor1 bin/pape bin/stack bin/thresh bin/two_q

bin/acc: src/acc.c src/acc_run.c src/types_gor.h src/parser_dh.c src/timer.c 
	$(CCOMP) $(CFLAGS) -o bin/acc src/acc_run.c

bin/dikq: src/dikq.c src/dikq_run.c src/types_bf.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/dikq src/dikq_run.c

bin/bf: src/bf.c src/bf_run.c src/types_bf.h src/parser_dh.c src/timer.c 
	$(CCOMP) $(CFLAGS) -o bin/bf src/bf_run.c

bin/bfp: src/bfp.c src/bfp_run.c src/types_bf.h src/parser_dh.c src/timer.c 
	$(CCOMP) $(CFLAGS) -o bin/bfp src/bfp_run.c

bin/dikb: src/dikb.c src/dikb_run.c src/types_db.h src/parser_db.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/dikb src/dikb_run.c

bin/dikba: src/dikba.c src/dikba_run.c src/types_db.h src/parser_db.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/dikba src/dikba_run.c

bin/dikbd: src/dikbd.c src/dikbd_run.c src/types_db.h src/parser_db.c src/timer.c 
	$(CCOMP) $(CFLAGS) -o bin/dikbd src/dikbd_run.c

bin/dikbm: src/dikbm.c src/dikbm_run.c src/types_db.h src/parser_db.c src/timer.c 
	$(CCOMP) $(CFLAGS) -o bin/dikbm src/dikbm_run.c

bin/dikh: src/dikh.c src/dikh_run.c src/types_dh.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/dikh src/dikh_run.c

bin/dikr: src/dikr.c src/dikr_run.c src/types_dr.h src/parser_db.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/dikr src/dikr_run.c

bin/dikf: src/dikf.c src/dikf_run.c src/types_f.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/dikf src/dikf_run.c -lm

bin/gor: src/gor.c src/gor_run.c src/types_gor.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/gor src/gor_run.c

bin/gor1: src/gor1.c src/gor1_run.c src/types_gor.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/gor1 src/gor1_run.c

bin/pape: src/pape.c src/pape_run.c src/types_bf.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/pape src/pape_run.c

bin/two_q: src/two_q.c src/two_q_run.c src/types_bf.h src/parser_dh.c src/timer.c 
	$(CCOMP) $(CFLAGS) -o bin/two_q src/two_q_run.c

bin/stack: src/stack.c src/stack_run.c src/types_bf.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/stack src/stack_run.c

bin/thresh: src/thresh.c src/thresh_run.c src/types_bf.h src/parser_dh.c src/timer.c
	$(CCOMP) $(CFLAGS) -o bin/thresh src/thresh_run.c
