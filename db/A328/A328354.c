/*
 * A328354  a(n)*S is the sum of all positive integers whose decimal expansion
 *          is up to n digits and uses seven distinct nonzero digits
 *          d1,d2,d3,d4,d5,d6,d7 such that d1+d2+d3+d4+d5+d6+d7=S.
 * https://oeis.org/A328354
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implementation for OEIS A328354
 *  a(0)=0, a(1)=1
 *  a(n) = 71*a(n-1) - 70*a(n-2) + 7^(n-1)   (n>1)
 *  This recurrence follows from the closed form:
 *      a(n) = (60*70^n - 69*7^n + 9)/3726
 *  The implementation keeps a memoisation table so that repeated
 *  calls with arbitrary indices are handled efficiently.  Intermediate
 *  calculations use 128‑bit arithmetic when seq_t is 64‑bit or smaller
 *  to avoid overflow; when seq_t is a larger arbitrary‑precision
 *  type, the native multiplication operators are used.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation state */
    static seq_t *buf  = NULL;   /* array of computed a(i) */
    static size_t sz   = 0;      /* number of elements stored   */
    static size_t cap  = 0;      /* allocated capacity          */
    static seq_t pow7       = 7; /* 7^(sz-1)   (so long as sz>=1) */

    /* initialisation */
    if (!buf) {
        cap = 4;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) abort();            /* allocation failure */
        buf[0] = (seq_t)0;            /* a(0) */
        buf[1] = (seq_t)1;            /* a(1) */
        sz    = 2;                    /* next index to compute */
        pow7  = (seq_t)7;             /* 7^(sz-1) = 7^1 */
    }

    /* grow the cache and compute missing values */
    while (sz <= (size_t)n) {
        /* ensure capacity */
        if (sz >= cap) {
            size_t newcap = cap * 2;
            seq_t *tmp = (seq_t *)realloc(buf, newcap * sizeof(seq_t));
            if (!tmp) abort();        /* allocation failure */
            buf = tmp;
            cap = newcap;
        }

        /* compute a(sz) using the recurrence */
        seq_t val;
        if (sizeof(seq_t) <= sizeof(uint64_t)) {
            /* 64‑bit or smaller: use 128‑bit intermediates */
            unsigned __int128 r1 = (unsigned __int128)buf[sz-1] * 71u;
            unsigned __int128 r2 = (unsigned __int128)buf[sz-2] * 70u;
            unsigned __int128 r  = r1 - r2 + (unsigned __int128)pow7;
            val = (seq_t)r;
        } else {
            /* arbitrary‑precision: direct 128‑bit mul is unnecessary */
            seq_t t1 = buf[sz-1] * 71;
            seq_t t2 = buf[sz-2] * 70;
            val = t1 - t2 + pow7;
        }

        buf[sz] = val;
        /* advance pow7 to 7^(sz) for the next iteration */
        pow7 *= (seq_t)7;
        sz++;
    }

    return buf[(size_t)n];
}
