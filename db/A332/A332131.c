/*
 * A332131  a(n) = (10^(2*n+1) - 1)/3 - 2*10^n.
 * https://oeis.org/A332131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = (10^(2*n+1) - 1)/3 - 2*10^n. Recurrence: a(n) =
 *            111*a(n-1) - 1110*a(n-2) + 1000*a(n-3) with initial values
 *            a(0)=1, a(1)=313, a(2)=33133
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cached = NULL;
    static idx_t cache_len = 0;

    /* initialise if needed */
    if (cached == NULL) {
        cache_len = 3;
        cached = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cached) return 0;   /* allocation failed; return 0 as fallback */
        cached[0] = 1;
        cached[1] = 313;
        cached[2] = 33133;
    }
    /* simple lookup if already computed */
    if (n < cache_len)
        return cached[n];

    /* grow cache to n+1 */
    size_t new_len = (size_t)(n + 1);
    seq_t *tmp = (seq_t *)realloc(cached, new_len * sizeof(seq_t));
    if (!tmp) return 0;   /* allocation failed; return 0 as fallback */
    cached = tmp;
    /* compute remaining terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* recurrence: a(i) = 111*a(i-1) - 1110*a(i-2) + 1000*a(i-3) */
        seq_t t1 = cached[i - 1] * 111;
        seq_t t2 = cached[i - 2] * 1110;
        seq_t t3 = cached[i - 3] * 1000;
        cached[i] = t1 - t2 + t3;
    }
    cache_len = new_len;
    return cached[n];
}
