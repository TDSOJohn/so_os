PandOS is the most recent of a series of educational operating systems created by Michael Goldweber and Renzo Davoli.
It runs on [uMPS3](https://github.com/virtualsquare/umps3), based around the MIPS R2/3000 microprocessor.

Structure
-------------------------
Inspired by the T.H.E. system outlined by Dijkstra in 1968, the OS is divided in a number of layers. Each layer *i* is an abstract machine to the layer *i+1*.

Existing layers:
- Level 0 (base hardware of uMPS3)
- Level 1 (additional BIOS services)
- Level 2 (Queue manager supporting queues of structures)


This directory contains the following supporting files for use in the Pandos Project:

General Purpose Files:
-------------------------
pandos_const.h: Header file contains utility constants & macro definitions
pandos_types.h: Header file contains utility types definitions


Pandos Phase 1 files:
-------------------------
Phase1Makefile: sample makefile for use in Phase 1 (rename as Makefile)
p1test.c: Source file for code to exercise and test a Phase 1 implementation
term0-Phase1.umps: Terminal0 output file for a correct Phase 1 implementation


Pandos Phase 2 files:
-------------------------
Phase2Makefile: sample makefile for use in Phase 2 (rename as Makefile)
p2test.c: Source file for code to exercise and test a Phase 2 implementation
term0-Phase2.umps: Terminal0 output file for a correct Phase 2 implementation


Pandos Phase 3 files:
-------------------------
Phase3Makefile: sample makefile for use in Phase 3 (rename as Makefile)
testPrograms subdirectory - which contains its own README file.

Usage
-------------------------
Compile command (use -d flag for additional debugging output):

```bash
make -C phase1 --file=Phase1Makefile
```
