/*
 * A275138  Number of n X 4 0..2 arrays with no element equal to any value at
 *          offset (-2,-2) (-1,-2) or (0,-1) and new values introduced in
 *          order 0..2.
 * https://oeis.org/A275138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A275138 – number of n × 4 arrays with given restrictions.
   Recurrence: a(1)=4, a(2)=48, a(3)=224, a(n)=4*a(n‑1)+4*a(n‑2) for n>3.  */

#include <stddef.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* static cache of computed values, 1‑based: seq[0] = a(1) */
    static seq_t *seq = NULL;
    static idx_t len  = 0;   /* number of values actually stored (might be less than cap) */
    static idx_t cap  = 0;   /* capacity of the allocated array */

    /* Constants promoted to seq_t */
    static const seq_t four   = 4;
    static const seq_t init1  = 4;
    static const seq_t init2  = 48;
    static const seq_t init3  = 224;

    /* Grow the array if required */
    if (n > cap) {
        /* grow to the next power of two that satisfies n */
        idx_t new_cap = cap ? cap << 1 : 1;
        while (new_cap < n) new_cap <<= 1;
        seq_t *new_seq = alloc_seq((size_t)new_cap);
        for (idx_t i = 0; i < len; ++i) new_seq[i] = seq[i];
        if (seq != NULL) free_seq(seq);
        seq = new_seq;
        cap = new_cap;
    }

    /* Compute any missing entries up to n */
    for (idx_t i = len; i < n; ++i) {
        if (i == 0) {
            seq[i] = init1;               /* a(1) */
        } else if (i == 1) {
            seq[i] = init2;               /* a(2) */
        } else if (i == 2) {
            seq[i] = init3;               /* a(3) */
        } else {
            seq[i] = four * seq[i - 1] + four * seq[i - 2];
        }
    }
    len = n;   /* now we have computed up to n */

    return seq[n - 1];   /* n is 1‑based */
}
