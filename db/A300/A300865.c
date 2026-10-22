/*
 * A300865  Signed recurrence over binary enriched p-trees: a(n) = (-1)^(n-1)
 *          + Sum_{x + y = n, 0 < x <= y < n} a(x) * a(y).
 * https://oeis.org/A300865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^(n-1) + Sum_{x + y = n, 0 < x <= y < n} a(x) * a(y)
 * Range:     u8 n <= 17, u16 n <= 27, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[1 .. last] holds a(1) .. a(last) */
static size_t cap = 0;           /* allocated size of the cache */
static idx_t last = 0;           /* largest n already computed */

seq_t a(idx_t n)
{
    if (n < 1) return 0; /* undefined for n < 1, defensive fallback */

    /* enlarge the cache if necessary; we keep index 0 unused,
       so capacity must be at least n + 1. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = cap ? cap << 1 : 32;
        /* ensure newcap >= n+1, then round up to a power of two */
        if (newcap < (size_t)n + 1) newcap = (size_t)n + 1;
        while (newcap < (size_t)n + 1) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap = newcap;
    }

    /* compute missing values up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = 1;
            continue;
        }
        seq_t sum = 0;
        idx_t half = i >> 1;                /* floor(i/2) */
        for (idx_t x = 1; x <= half; ++x) {
            sum += cache[(size_t)x] * cache[(size_t)(i - x)];
        }
        if (((i - 1) & 1) != 0) {           /* (-1)^(i-1) = -1 */
            sum -= 1;
        } else {                            /* (-1)^(i-1) = +1 */
            sum += 1;
        }
        cache[(size_t)i] = sum;
    }

    last = n;
    return cache[(size_t)n];
}
