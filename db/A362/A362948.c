/*
 * A362948  Numbers whose sum of (distinct) prime divisors (A008472) equals 5.
 * https://oeis.org/A362948
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 73, u32 n <= 286, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A362948
   Sequence: numbers whose sum of distinct prime divisors equals 5,
   i.e. the sorted union of {5^k : k>0} and {2^j·3^k : j,k>0}. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n)
{
    /* Cache of already computed terms */
    static seq_t *vals = NULL;
    static size_t vals_cap = 0;
    static idx_t vals_len = 0;

    /* Next power of 5 to be considered */
    static seq_t next5 = 0;
    static bool init5 = false;

    /* Array of 2^j*3^k terms (j,k>0) */
    static seq_t *B = NULL;
    static size_t B_cap = 0;
    static idx_t B_len = 0;
    static idx_t B_ptr = 0;
    static idx_t i2 = 0;
    static idx_t i3 = 0;
    static bool initB = false;

    /* Initialise storage for sequence values */
    if (!vals) {
        vals_cap = 16;
        vals = (seq_t *)calloc(vals_cap, sizeof(seq_t));
    }

    /* Initialise next power of 5 */
    if (!init5) {
        next5 = (seq_t)5;
        init5 = true;
    }

    /* Initialise table of 2^j*3^k values */
    if (!initB) {
        B_cap = 16;
        B = (seq_t *)calloc(B_cap, sizeof(seq_t));
        B[0] = (seq_t)6;          /* 2^1 * 3^1 */
        B_len = 1;
        B_ptr = 0;
        i2 = 0;
        i3 = 0;
        initB = true;
    }

    /* Generate terms until we reach the requested index */
    while (vals_len < n) {

        /* Enlarge B if needed by generating next element */
        while (B_ptr == B_len) {
            seq_t cand2 = B[i2] * (seq_t)2;
            seq_t cand3 = B[i3] * (seq_t)3;
            seq_t nextB;
            if (cand2 < cand3) {
                nextB = cand2;
                ++i2;
            } else if (cand3 < cand2) {
                nextB = cand3;
                ++i3;
            } else {               /* equal */
                nextB = cand2;
                ++i2;
                ++i3;
            }
            grow_seq(&B, &B_cap, B_len + 1);
            B[B_len++] = nextB;
        }

        seq_t nextB = B[B_ptr];
        seq_t val;

        if (next5 < nextB) {
            val = next5;
            next5 = next5 * (seq_t)5;
        } else {
            val = nextB;
            ++B_ptr;
        }

        grow_seq(&vals, &vals_cap, vals_len + 1);
        vals[vals_len++] = val;
    }

    return vals[n - 1];
}
