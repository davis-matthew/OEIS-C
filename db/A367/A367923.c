/*
 * A367923  Expansion of e.g.f. 1/(1 - x + 3*log(1 - x)).
 * https://oeis.org/A367923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = n*a(n-1) + 3*Sum_{k=1..n}
 *            (k-1)!*binomial(n,k)*a(n-k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Ensure cache[0] is initialized. */
    if (cache_len == 0) {
        cache_len = 1;
        cache = (seq_t*)malloc(sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;            /* a(0) = 1 */
    }

    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate n. */
    seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
    if (!tmp) exit(1);
    cache = tmp;

    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t sum = 0;
        seq_t C = (seq_t)i;      /* C(i,1) */
        seq_t fact = (seq_t)1;   /* (k-1)! for k=1 */

        for (idx_t k = 1; k <= i; ++k) {
            seq_t term = fact * C;
            seq_t addend = term * cache[i - k];
            sum += addend;

            if (k < i) {
                fact *= (seq_t)k;                    /* fact becomes k! for next k+1 */
                seq_t numerator = C * ((seq_t)(i - k));
                C = numerator / ((seq_t)(k + 1));    /* next binomial value */
            }
        }

        seq_t base = ((seq_t)i) * cache[i - 1];
        cache[i] = base + ((seq_t)3 * sum);
    }

    cache_len = n + 1;
    return cache[n];
}
