/*
 * A255431  Numbers n such that integers n through n+6 and their squares all
 *          lack the digit 1 in their decimal expansion.
 * https://oeis.org/A255431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined by a brute‑force search: 
   the nth term is the nth integer n such that the seven consecutive
   integers n … n+6 and all their squares contain no decimal digit 1.
   All solutions are congruent to 2 mod 10, so we only test numbers
   ending in 2. This implementation stores previously found terms
   in a static array so that calls in arbitrary order run quickly. */

#include <stdbool.h>
#include <stddef.h>

/* Test whether a number contains the decimal digit 1. */
static inline bool has_digit_one(seq_t x)
{
    /* The literal 1 and the modulo/division by 10 are accepted for all
       integer types seq_t.  For an arbitrary‑precision class the
       operators are defined accordingly. */
    while (x != (seq_t)0) {
        if ((x % (seq_t)10) == (seq_t)1)
            return true;
        x /= (seq_t)10;
    }
    return false;
}

/* Compute the nth term of the sequence (1‑based indexing). */
seq_t a(idx_t n)
{
    /* Static cache – enough for the required range (up to about 10 000
       terms).  The array size is modest (20000) and keeps the program
       well below memory limits even for the arbitrary‑precision build. */
    enum { CACHE_CAP = 20000 };
    static seq_t cache[CACHE_CAP];
    static size_t cached = 0;          /* number of terms stored so far */
    static seq_t search = (seq_t)2;    /* next candidate ending in 2 */

    /* Extend the cache until it contains at least n terms. */
    while (cached < n) {
        seq_t cand = search;
        bool ok = false;

        /* Search successive candidates ending in 2 until a valid one is
           found. */
        while (!ok) {
            /* Quick check: candidate itself must not contain 1. */
            if (!has_digit_one(cand)) {
                ok = true;

                /* Check the seven consecutive numbers cand … cand+6. */
                for (idx_t off = 1; off < 7 && ok; ++off) {
                    if (has_digit_one(cand + (seq_t)off))
                        ok = false;
                }

                /* If still fine, check the squares of the seven numbers. */
                if (ok) {
                    for (idx_t off = 0; off < 7 && ok; ++off) {
                        seq_t x = cand + (seq_t)off;
                        seq_t sq = x * x;
                        if (has_digit_one(sq))
                            ok = false;
                    }
                }
            }

            /* Move to the next number ending in 2. */
            cand += (seq_t)10;
        }

        /* Store the found term.  cache should never overflow because
           CACHE_CAP is chosen generously for all tested n. */
        cache[cached++] = cand - (seq_t)10;  /* cand has been incremented once */
        search = cand;                       /* start next search after this value */
    }

    return cache[n - 1];
}
