/*
 * A297645  Hexagonal numbers (A000384) in which parity of digits alternates.
 * https://oeis.org/A297645
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 201, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   parity_alternate
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A297645: hexagonal numbers whose decimal digits have alternating parity.
    For each n ≥ 1, the function returns the n‑th hexagonal number H(k)=k(2k−1)
    that satisfies the alternation property:
    adjacent decimal digits must differ in even/odd parity.  The sequence
    values are generated on demand and cached for future calls.  */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/parity_alternate.h"

seq_t a(idx_t n)
{
    /* static cache of sequence elements and helpers for incremental
       generation.  The invariant is that when the function returns,
       seq[0..count-1] contains the first `count` terms of the sequence,
       and hex_index points to the next k to test for k(2k−1).           */
    static seq_t *seq   = NULL;
    static size_t cap   = 0;
    static idx_t count  = 0;
    static idx_t hex_index = 1;   /* next hexagonal index to examine       */

    /* Grow the cache if necessary */
    while (count < n) {
        seq_t k = (seq_t)hex_index;                     /* hexagonal index   */
        seq_t h = k * ((k << 1) - 1);                   /* hexagonal number */

        if (parity_alternate(h)) {                      /* property satisfied? */
            if (count == cap) {                         /* expand if full   */
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq, new_cap * sizeof(seq_t));
                if (!tmp) { /* malloc failure: abort (undefined in build) */
                    exit(1);
                }
                seq = tmp;
                cap = new_cap;
            }
            seq[count++] = h;                           /* store term      */
        }
        ++hex_index;                                   /* next k          */
    }

    return seq[n - 1];
}
