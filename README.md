mchess
======

This is a simple (minimalistic) chess engine, but functional never the less.
It is meant as a starting point for writing your own chess engine. Use it for
inspiration :-)

This is actually a part of the answer:
http://www.quora.com/How-do-I-get-started-with-my-own-chess-engine/answer/Michael-J%C3%B8rgensen-2

Features
========

- Includes a simple alpha-beta search strategy.
- Detects check-mate and stale-mate.
- A `perft N` command for testing the move generator.
- Searches a fixed depth of five ply (the root move plus four), followed by a capture-only quiescence search.
- A simple console (ASCII) user interface.

Documentation
=============

The design is documented in the file DOC.html.

The source code has comments in most places.  Otherwise, the best place for a high level description is the
above link.
Additionally, the directory doc contains some additional information on writing chess engines.


TODO
====
- Castling
- En passant
- UCI interface (for GUI)
- Time control
- Opening book

