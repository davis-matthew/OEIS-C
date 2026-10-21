/*
 * A260702  Numbers n such that 3*n and n^2 have the same digit sum.
 * https://oeis.org/A260702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 43, u32 n <= 2560, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A260702
   a(n) = the nth non‑negative integer n such that digit_sum(3*n) == digit_sum(n*n).
   All terms are multiples of 3; the sequence contains 0,3,6,9,12,... and
   whenever n is in the sequence so is 10*n. */

/* Implementation note:
   The sequence is generated on demand by testing successive multiples of 3.
   A static dynamic array stores the discovered terms so repeated calls
   reuse earlier work.  The algorithm is O(n) for the nth term with low
   constant factors. */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_sum.h"

/* Static cache of sequence values */
static seq_t *seq = NULL;      /* array of terms */
static size_t seq_cap = 0;     /* allocated capacity */
static size_t seq_len = 0;     /* number of stored terms */
static seq_t   candidate = 0;  /* next multiple of 3 to test */

/* Grow the cache to accommodate at least 'need' more terms. */
static void grow_cache(size_t need)
{
    size_t new_cap = seq_cap ? seq_cap * 2 : 16;
    while (new_cap - seq_len < need) {
        new_cap *= 2;
    }
    seq = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
    seq_cap = new_cap;
}

/* Return the nth element (1‑based) of the sequence. */
seq_t a(idx_t n)
{
    /* Convert the index to a C size_t for internal use. */
    size_t target = (size_t)n;

    /* Ensure we have computed enough terms. */
    while (seq_len < target) {
        /* Check the current candidate. */
        seq_t val = candidate;                /* a(n) candidate */
        seq_t thr = 3 * val;                  /* 3*n   */
        seq_t sq  = val * val;                /* n^2   */

        if (digit_sum(thr) == digit_sum(sq)) {
            /* Store the term. */
            if (seq_len == seq_cap) {
                grow_cache(1);
            }
            seq[seq_len++] = val;
        }

        /* Advance to the next multiple of 3. */
        candidate += 3;
    }

    /* Retrieve the requested term. */
    return seq[target - 1];
}
