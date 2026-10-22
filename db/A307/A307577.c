/*
 * A307577  Number of Motzkin meanders of length n with an odd number of
 *          peaks.
 * https://oeis.org/A307577
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 42, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A307577
   a(n) = number of Motzkin meanders of length n with an odd number of peaks
   (computed by dynamic programming). */

#include <stdlib.h>
#include <string.h>

/* DP states:
   0 = even number of peaks, last step not U
   1 = even number of peaks, last step U
   2 = odd  number of peaks, last step not U
   3 = odd  number of peaks, last step U
   For each state we keep an array of counts indexed by the current height. */

seq_t a(idx_t n)
{
    /* Persistent storage shared between calls. */
    static seq_t *res = NULL;          /* a(0..maxComputed)          */
    static idx_t resCap = 0;           /* capacity of res array      */
    static idx_t maxComputed = 0;      /* largest n for which a(n) is known */

    static seq_t *cur[4]  = {NULL, NULL, NULL, NULL}; /* state of step k    */
    static seq_t *nxt[4]  = {NULL, NULL, NULL, NULL}; /* state of step k+1 */
    static idx_t dpCap = 0;                         /* capacity of state arrays  */

    /* Ensure res array can hold up to n. */
    if (n + 1 > resCap) {
        idx_t newCap = n + 1;
        res = (seq_t*)realloc(res, newCap * sizeof(seq_t));
        resCap = newCap;
        /* Initially all entries are 0 because realloc does not clear memory.  */
        if (maxComputed == 0) {
            /* Still need to clear the new region explicitly. */
            memset(res, 0, newCap * sizeof(seq_t));
        }
    }

    /* Ensure DP state arrays can hold heights up to n. */
    if (n + 1 > dpCap) {
        idx_t newCap = n + 1;
        for (int s = 0; s < 4; ++s) {
            cur[s]  = (seq_t*)realloc(cur[s],  newCap * sizeof(seq_t));
            nxt[s]  = (seq_t*)realloc(nxt[s],  newCap * sizeof(seq_t));
            /* Zero the newly added elements. */
            memset(cur[s] + dpCap, 0, (newCap - dpCap) * sizeof(seq_t));
            memset(nxt[s] + dpCap, 0, (newCap - dpCap) * sizeof(seq_t));
        }
        dpCap = newCap;
    }

    /* One‑time initialisation of the DP for step 0. */
    if (maxComputed == 0) {
        /* All counts are 0 except the empty path with height 0, even parity,
           last step not U. */
        cur[0][0] = 1;
        for (int s = 1; s < 4; ++s)
            cur[s][0] = 0;
        res[0] = 0;          /* a(0) = 0 by definition */
        maxComputed = 0;
    }

    /* Extend the DP up to the requested n. */
    for (idx_t step = maxComputed + 1; step <= n; ++step) {
        idx_t maxPrevHeight = step - 1;

        /* Zero next state for all heights that may be reached in this step. */
        for (int s = 0; s < 4; ++s)
            memset(nxt[s], 0, (step + 1) * sizeof(seq_t));

        for (idx_t h = 0; h <= maxPrevHeight; ++h) {
            seq_t c0 = cur[0][h]; /* even, last not U */
            seq_t c1 = cur[1][h]; /* even, last U    */
            seq_t c2 = cur[2][h]; /* odd , last not U */
            seq_t c3 = cur[3][h]; /* odd , last U    */

            /* Horizontal step H : height unchanged, parity unchanged,
               last step becomes not U. */
            if (c0) nxt[0][h] += c0;
            if (c1) nxt[0][h] += c1;
            if (c2) nxt[2][h] += c2;
            if (c3) nxt[2][h] += c3;

            /* Up step U : height+1, last step becomes U,
               parity unchanged. */
            if (c0) nxt[1][h + 1] += c0;
            if (c1) nxt[1][h + 1] += c1;
            if (c2) nxt[3][h + 1] += c2;
            if (c3) nxt[3][h + 1] += c3;

            /* Down step D : possible only if h > 0.
               A peak occurs iff the previous step was U. */
            if (h > 0) {
                /* Peak (previous step U): parity flips. */
                if (c1) nxt[2][h - 1] += c1; /* even -> odd  */
                if (c3) nxt[0][h - 1] += c3; /* odd  -> even */

                /* No peak (previous step not U): parity keeps same. */
                if (c0) nxt[0][h - 1] += c0; /* even -> even */
                if (c2) nxt[2][h - 1] += c2; /* odd  -> odd  */
            }
        }

        /* Total number of paths of length 'step' ending at any height
           with an odd number of peaks. */
        seq_t totalOdd = 0;
        for (idx_t h = 0; h <= step; ++h) {
            totalOdd += nxt[2][h];
            totalOdd += nxt[3][h];
        }
        res[step] = totalOdd;

        /* Prepare for the next iteration: swap cur and nxt. */
        for (int s = 0; s < 4; ++s) {
            seq_t *tmp = cur[s];
            cur[s] = nxt[s];
            nxt[s] = tmp;
        }

        maxComputed = step;
    }

    return res[n];
}
