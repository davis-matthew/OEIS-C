/*
 * A398788  Minimum number of rainbow spanning trees of K_n, over all
 *          JL-colorings.
 * https://oeis.org/A398788
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = s*(n-s)*a(s)*a(n-s), where s is a power of 2 between n/3
 *            and 2*n/3. Initial values: a(2) = 1, a(3) = 2
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 21, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   seq_mul, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Core implementation uses memoisation and helper big‑integer utilities. */
#include <stdint.h>
#include <stddef.h>
#include "../utils/seq_mul.h"
#include "../utils/seq_realloc.h"

/* static cache for computed terms */
static seq_t *cache = NULL;      /* cache[0]..cache[cap-1] */
static size_t cache_cap = 0;     /* allocated capacity */

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)                /* undefined below offset, return 0 */
        return (seq_t)0;

    /* ensure cache large enough for index n */
    if (n + 1 > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        cache = seq_realloc(cache, cache_cap, new_cap);
        cache_cap = new_cap;
    }

    /* return cached value if already computed */
    if (cache[(size_t)n] != 0)
        return cache[(size_t)n];

    /* base cases */
    if (n == 2) return cache[(size_t)2] = (seq_t)1;
    if (n == 3) return cache[(size_t)3] = (seq_t)2;

    /* find a suitable power of two s in [ceil(n/3), floor(2n/3)] */
    idx_t min = (n + 2) / 3;          /* ceil(n/3) */
    idx_t max = (2 * n) / 3;          /* floor(2n/3) */
    idx_t s = 1;
    while (s < min)
        s <<= 1;
    if (s > max) {                   /* fallback to largest power <= max */
        s = 1;
        while (s <= max / 2)
            s <<= 1;
    }

    idx_t t = n - s;

    /* recursively compute a(s) and a(t) */
    seq_t val_s = a(s);
    seq_t val_t = a(t);

    /* compute a(n) = s*(n-s)*a(s)*a(t) using big‑integer multiplication */
    seq_t temp = seq_mul((seq_t)s, (seq_t)(n - s));
    seq_t res  = seq_mul(temp,   val_s);
    res        = seq_mul(res,    val_t);

    cache[(size_t)n] = res;
    return res;
}
