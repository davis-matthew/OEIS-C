/*
 * A359380  Numbers that are neither multiples of 3 nor of the form 4u+2.
 * https://oeis.org/A359380
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A359380: Numbers that are neither multiples of 3
 * nor of the form 4u+2.  In other words,
 * a(n) is the n-th positive integer such that
 *       x % 3 != 0  and  x % 4 != 2   (x ≡ 1,4,5,7,8,11 (mod 12))
 * This algorithm builds the sequence incrementally and caches the
 * results so that repeated calls are fast.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Lazily initialised cache */
    static seq_t *seq = NULL;
    static size_t seq_len  = 0;   /* number of entries stored */
    static size_t seq_cap  = 0;   /* capacity of seq */
    static seq_t last_checked = 0;/* last integer examined */

    /* Validate n (offset 1).  Undefined behaviour for n == 0. */
    if (n == 0) return (seq_t)0;

    /* Initialise cache on first call. */
    if (seq == NULL) {
        seq_cap = 16;
        seq = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
    }

    /* Grow the cache as needed. */
    while (seq_len < n) {
        /* Advance to next integer to test. */
        last_checked += 1;

        /* Check if it fits the definition. */
        if ((last_checked % 3 != 0) && (last_checked % 4 != 2)) {
            /* Append to cache. */
            if (seq_len == seq_cap) {
                seq_cap <<= 1;                       /* double capacity */
                seq = (seq_t *)realloc(seq, seq_cap * sizeof(seq_t));
            }
            seq[seq_len++] = last_checked;
        }
    }

    /* Return the requested element (1-based index). */
    return seq[n - 1];
}
