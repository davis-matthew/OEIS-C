/*
 * A307992  G.f. A(x) satisfies: A(x) = x + x^2 * (1 + A(x) + 2*A(x^2) +
 *          3*A(x^3) + ...).
 * https://oeis.org/A307992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = a(2) = 1 a(n) = Sum_{d | (n-2)} d * a((n-2)/d) for n ≥ 3
 * Range:     u8 n <= 19, u16 n <= 83, u32 n <= 681, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* No external libraries needed other than <stdlib.h> for memory handling. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑indexed array of computed terms     */
static size_t cache_len = 0;         /* current allocated length (index last+1) */

/* Compute a(n) for all 1 ≤ k ≤ n.
   The function is memoised: previously computed entries are reused,
   and new entries are generated on demand. */
static void build_to(idx_t n)
{
    if (n + 1 <= cache_len) return;           /* already available */

    /* enlarge the cache */
    size_t new_len = (size_t)(n) + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) abort();          /* should never happen in the intended setting */
    cache = tmp;

    /* initialise from the previous size up to n */
    for (idx_t i = (idx_t)cache_len; i <= n; ++i)
    {
        if (i == 0) { cache[0] = 0; continue; }          /* placeholder */

        if (i == 1 || i == 2)
        {
            cache[i] = 1;
            continue;
        }

        idx_t t = i - 2;
        seq_t sum = 0;
        for (idx_t d = 1; d * d <= t; ++d)
        {
            if (t % d == 0)
            {
                idx_t d1 = d;
                idx_t d2 = t / d;

                /* term for divisor d1 */
                seq_t term1 = (seq_t)d1 * cache[d2];
                sum += term1;

                /* term for complementary divisor d2 (if distinct) */
                if (d1 != d2)
                {
                    seq_t term2 = (seq_t)d2 * cache[d1];
                    sum += term2;
                }
            }
        }
        cache[i] = sum;
    }
    cache_len = new_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* sequence starts at index 1; zeros elsewhere */
    build_to(n);
    return cache[n];
}
