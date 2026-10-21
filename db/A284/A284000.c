/*
 * A284000  a(n) = a(a(n-A002828(n))) + a(n-a(n-A002828(n))) with a(1) = a(2)
 *          = a(3) = 1, where A002828(n) = the least number of squares that
 *          add up to n.
 * https://oeis.org/A284000
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(a(n-A002828(n))) + a(n-a(n-A002828(n))),
 *            a(1)=a(2)=a(3)=1
 * Range:     u8 n <= 413, u16 n <= 9747, u32 n <= 9747, u64 n <= 9747,
 *            u128 n <= 9747, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

/* ---------- integer square root for idx_t -------------------------------- */
static inline idx_t isqrt_u64(idx_t n)
{
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

/* ---------- least number of squares (A002828) ---------------------------- */
static idx_t min_squares(idx_t n)
{
    if (n == 0) return 0;
    idx_t r = isqrt_u64(n);
    if (r * r == n) return 1;                            /* perfect square */

    /* test 2 squares */
    for (idx_t k = 1; k <= r; ++k) {
        idx_t rem = n - k * k;
        idx_t s = isqrt_u64(rem);
        if (s * s == rem) return 2;                      /* two squares */
    }

    /* test 3 or 4 squares using Legendre's theorem */
    idx_t t = n;
    while (t % 4 == 0) t /= 4;
    return (t % 8 == 7) ? 4 : 3;
}

/* ---------- memoised sequence cache ------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

static void ensure_cache(idx_t n)
{
    if (n <= cache_cap) return;
    size_t new_cap = (cache_cap == 0) ? 32 : cache_cap;
    while (new_cap < (size_t)n) new_cap <<= 1;
    seq_t *new_cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!new_cache) abort();                                   /* out of memory */
    if (new_cap > cache_cap) {
        /* initialise newly allocated entries to zero */
        memset(new_cache + cache_cap + 1, 0,
               (new_cap - cache_cap) * sizeof(seq_t));
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* ---------- the required function -------------------------------------- */
seq_t a(idx_t n)
{
    if (n <= 3) return 1;

    ensure_cache(n);

    /* initialise base values if not already set */
    if (cache[1] == 0) cache[1] = 1;
    if (cache[2] == 0) cache[2] = 1;
    if (cache[3] == 0) cache[3] = 1;

    for (idx_t i = 4; i <= n; ++i) {
        if (cache[i] != 0) continue;          /* already computed */

        /* A002828(i) */
        idx_t k = min_squares(i);

        /* p = i - k,   sub = a(p) */
        idx_t p = i - k;
        seq_t sub = cache[p];

        /* a_a = a(sub) */
        seq_t a_a = cache[(idx_t)sub];

        /* q = i - sub,   sub2 = a(q) */
        idx_t q = i - (idx_t)sub;
        seq_t sub2 = cache[q];

        cache[i] = a_a + sub2;
    }

    return cache[n];
}
