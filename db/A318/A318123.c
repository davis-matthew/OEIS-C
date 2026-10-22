/*
 * A318123  a(0) = a(1) = 1; a(n) = Sum_{k=0..n-2} lcm(a(k), a(n-k-2)).
 * https://oeis.org/A318123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1; a(n) = Σ_{k=0}^{n-2} lcm(a(k),a(n-k-2))
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 32, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   ensure_cache, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/gcd.h"
#include "../utils/ensure_cache.h"

static inline seq_t lcm_value(seq_t x, seq_t y)
{
    seq_t g = gcd(x, y);
    return (x / g) * y;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cap  = 0;

    if (n < 2) {
        /* base values */
        return (seq_t)1;
    }

    /* ensure we have space for indices up to n */
    ensure_cache(n, &cache, &cap);

    /* return already computed value */
    if (cache[n] != 0) {
        return cache[n];
    }

    /* compute sum of lcms, using symmetry to half the work */
    seq_t sum = 0;
    idx_t i = 0;
    idx_t j = n - 2;
    while (i < j) {
        seq_t l = lcm_value(a(i), a(j));
        sum += l;
        sum += l;          /* add twice, for the symmetric term */
        ++i;
        --j;
    }
    if (i == j) {
        /* middle term: lcm(x,x) == x */
        sum += a(i);
    }

    cache[n] = sum;
    return sum;
}
