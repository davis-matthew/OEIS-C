/*
 * A391830  a(n) = (1/4) * Sum_{k=0..floor(n/2)} (k+2) *
 *          binomial(2*k+2,2*n-4*k+1).
 * https://oeis.org/A391830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 37, u64 n <= 75, u128 n <= 153,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS sequence A391830
 *
 *  a(n) = 4*a(n-2) + 4*a( n-3) – 6*a(n-4) – 4*a(n-5) – 2*a(n-6)
 *          – 4*a(n-7) – 5*a(n-8) + 8*a(n-9) – 6*a(n-10) + 4*a(n-11) – a(n-12)
 *
 *  Initial values (n = 0 … 11):
 *    a(0)=1, a(1)=0, a(2)=3, a(3)=3, a(4)=6, a(5)=20,
 *    a(6)=16, a(7)=70, a(8)=85, a(9)=190,
 *    a(10)=399, a(11)=565
 *
 *  The implementation uses a simple dynamic cache.  Each call
 *  runs in O(n) time for the largest n that has been requested
 *  so far and uses only the operators allowed on seq_t.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of already computed values.  The first 12 terms are
     * stored in init and used to initialise the cache. */
    static const seq_t init[12] = {
        1, 0, 3, 3, 6, 20, 16, 70, 85, 190, 399, 565
    };
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;  /* number of entries stored (may be 0) */

    /* Handle very small indices directly. */
    if (n < 12) {
        return init[n];
    }

    /* Grow the cache if necessary. */
    if (n + 1 > cached_len) {
        /* Allocate a larger array.  The explicit cast is required for
         * C++ where realloc returns void*.  In C the cast is also
         * permitted but not required. */
        size_t new_len = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) {            /* allocation failed – return 0 as a safe fallback */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise new entries.  For indices < 12 we copy the
         * hard‑coded initial values; otherwise we use the recurrence. */
        for (size_t i = cached_len; i < new_len; ++i) {
            if (i < 12) {
                cache[i] = init[i];
            } else {
                /* Compute a(i) using the recurrence.
                 * All arithmetic is performed with seq_t operands
                 * so that the code works for built‑in integer types
                 * as well as for arbitrary‑precision integer classes. */
                seq_t pos = 0;
                pos += (seq_t)4 * cache[i - 2];
                pos += (seq_t)4 * cache[i - 3];
                pos += (seq_t)8 * cache[i - 9];
                pos += (seq_t)4 * cache[i - 11];

                seq_t neg = 0;
                neg += (seq_t)6 * cache[i - 4];
                neg += (seq_t)4 * cache[i - 5];
                neg += (seq_t)2 * cache[i - 6];
                neg += (seq_t)4 * cache[i - 7];
                neg += (seq_t)5 * cache[i - 8];
                neg += (seq_t)6 * cache[i - 10];
                neg += (seq_t)1 * cache[i - 12];

                cache[i] = pos - neg;
            }
        }
        cached_len = new_len;
    }

    return cache[n];
}
