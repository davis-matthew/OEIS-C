/*
 * A373185  G.f. A(x) satisfies A(x) = 1/(1 - x)^2 - 1 + A(x^4).
 * https://oeis.org/A373185
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n+1 (for n mod 4 ≠ 0) a(n) = n+1 + a(n/4) (for n mod 4 =
 *            0) Offset: 1 (i.e. a(1) is the first term)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* 1‑based array: cache[i] = a(i) */
    static idx_t  cap   = 0;      /* highest index that may be stored */
    static idx_t  last  = 0;      /* highest index that has been computed */

    /* enlarge cache if needed */
    if (n > cap) {
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = n;
    }

    /* compute all values up to n that are missing */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t val = (seq_t)(i + 1);
        if ((i & 3) == 0) {            /* i % 4 == 0 */
            val += cache[i / 4];
        }
        cache[i] = val;
    }
    last = n;

    return cache[n];
}
