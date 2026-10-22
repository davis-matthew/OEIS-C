/*
 * A369807  Expansion of 1/(1 - x^4/(1-x)^7).
 * https://oeis.org/A369807
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} binomial(n-1 + 3k, n-4k) *
 *            Equivalent recurrence: a(n) = 7*a(n-1) - 21*a(n-2) + 35*a(n-3) -
 *            * 34*a(n-4) + 21*a(n-5) - 7*a(n-6) + a(n-7) * Initial values:
 *            a(0)=1, a(1)=0, a(2)=0, a(3)=0, a(4)=1, a(5)=7, * a(6)=28,
 *            a(7)=84
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *cache = NULL;      /* Cached computed values */
static size_t cache_len = 0;     /* Number of elements currently in cache */

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t new_cap = (size_t)n + 1;              /* must fit in size_t */
        seq_t *tmp = seq_realloc(cache, cache_len, new_cap);
        cache = tmp;
        /* Compute any new entries */
        for (idx_t i = (idx_t)cache_len; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)1;
            } else if (i == 1 || i == 2 || i == 3) {
                cache[i] = (seq_t)0;
            } else if (i == 4) {
                cache[i] = (seq_t)1;
            } else if (i == 5) {
                cache[i] = (seq_t)7;
            } else if (i == 6) {
                cache[i] = (seq_t)28;
            } else if (i == 7) {
                cache[i] = (seq_t)84;
            } else {
                /* Use positive/negative decomposition to avoid negative
                 * intermediates in unsigned arithmetic. */
                seq_t pos = (seq_t)0;
                seq_t neg = (seq_t)0;

                pos = pos + (seq_t)7    * cache[i-1];
                pos = pos + (seq_t)35   * cache[i-3];
                pos = pos + (seq_t)21   * cache[i-5];
                pos = pos +          cache[i-7];

                neg = neg + (seq_t)21   * cache[i-2];
                neg = neg + (seq_t)34   * cache[i-4];
                neg = neg + (seq_t)7    * cache[i-6];

                cache[i] = pos - neg;   /* pos >= neg by recurrence */
            }
        }
        cache_len = new_cap;
    }
    return cache[n];
}
