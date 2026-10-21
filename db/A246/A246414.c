/*
 * A246414  Positive integers k satisfying csc(k) < csc(k+1) > csc(k+2).
 * https://oeis.org/A246414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 122, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A246414: a(n) = k such that csc(k) < csc(k+1) > csc(k+2).
   The code searches integers k by evaluating sin(k) with long double
   arithmetic (via sinl).  Matching k values are cached so that
   subsequent calls need not recompute earlier terms. */

#include <stdlib.h>
#include <math.h>

seq_t a(idx_t n)
{
    /* Cached values of the sequence.  Because n can be requested in
       arbitrary order, the cache grows as needed. */
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    /* next_k holds the next integer k to test. */
    static idx_t next_k = 1;

    if (len < n) {
        /* Initialise cache if first time. */
        if (cap == 0) {
            cap = 128;
            cache = (seq_t *)realloc(NULL, cap * sizeof(seq_t));
        }
        /* Generate terms until we have at least n of them. */
        while (len < n) {
            long double s0 = sinl((long double)next_k);
            long double s1 = sinl((long double)(next_k + 1));
            long double s2 = sinl((long double)(next_k + 2));

            long double c0 = 1.0L / s0;
            long double c1 = 1.0L / s1;
            long double c2 = 1.0L / s2;

            if (c0 < c1 && c1 > c2) {
                if (len == cap) {
                    cap *= 2;
                    cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                }
                cache[len++] = (seq_t)next_k;
            }
            ++next_k;
        }
    }
    /* Return the requested term.  (n >= 1 guaranteed.) */
    return cache[n - 1];
}
