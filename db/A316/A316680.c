/*
 * A316680  The integer 1358 and its infinite continuation (when iterating the
 *          rule explained in A316650 and in the Comment section here).
 * https://oeis.org/A316680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1358; for n > 1, let s = digit_sum(a(n‑1)); a(n) =
 *            concat( a(n‑1)/s , a(n‑1)%s ); /* q * 10^k + r, k = digit_len(r)
 * Range:     u8 none, u16 n <= 3, u32 n <= 20, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   digit_len, digit_sum, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/digit_sum.h"       /* returns seq_t */
#include "../utils/digit_len.h"       /* returns int */
#include "../utils/pow10.h"           /* seq_t pow10(idx_t) */

seq_t a(idx_t n)
{
    /* cache is 1‑indexed: cache[0] == a(1) */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;

    if (size == 0) {                 /* first time, initialize */
        cap   = 64;
        cache = (seq_t *)malloc(sizeof(seq_t) * cap);
        if (!cache) {
            /* perror omitted; return 0 on allocation failure */
            return 0;
        }
        cache[0] = (seq_t)1358;     /* a(1) */
        size = 1;
    }

    /* extend cache until we reach index n */
    while (size < n) {
        seq_t prev = cache[size - 1];

        seq_t s = digit_sum(prev);          /* sum of decimal digits of prev */
        seq_t q = prev / s;
        seq_t r = prev % s;

        int k = digit_len(r);               /* number of decimal digits of r */

        seq_t newv = q * pow10((idx_t)k) + r;

        if (size >= cap) {                  /* need more space */
            cap *= 2;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * cap);
            if (!tmp) {
                /* realloc failed, keep old cache to avoid corruption */
                return 0;
            }
            cache = tmp;
        }

        cache[size++] = newv;
    }

    return cache[n - 1];
}
