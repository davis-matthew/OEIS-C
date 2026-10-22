/*
 * A300084  Number of nX3 0..1 arrays with every element equal to 0, 1, 3, 5,
 *          6, 7 or 8 king-move adjacent elements, with upper left element
 *          zero.
 * https://oeis.org/A300084
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3) -10*a(n-4) +2*a(n-5)
 *            +2*a(n-6) +2*a(n-7) -3*a(n-8) -3*a(n-9) +12*a(n-10) -4*a(n-11)
 *            +17*a(n-12) -3*a(n-13) -5*a(n-14) + a(n-15) -12*a(n-16)
 *            -6*a(n-17) -2*a(n-18) -3*a(n-19) - a(n-20), for n > 21. Initial
 *            values: a(1)=3, a(2)=4, a(3)=1, a(4)=4, a(5)=10, a(6)=6,...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 75, u128 n <= 148,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static const seq_t base[22] = {                      /* 1‑indexed */
    0ULL,                                           /* dummy a(0) */
    3ULL, 4ULL, 1ULL, 4ULL, 10ULL, 6ULL, 19ULL, 41ULL,
    32ULL, 106ULL, 177ULL, 204ULL, 567ULL, 854ULL,
    1301ULL, 3067ULL, 4579ULL, 8193ULL, 16931ULL,
    26544ULL, 50620ULL
};

static const long long coeff[20] = {
    2LL, 1LL, 2LL, -10LL, 2LL, 2LL, 2LL,
    -3LL, -3LL, 12LL, -4LL, 17LL, -3LL, -5LL,
    1LL, -12LL, -6LL, -2LL, -3LL, -1LL
};

/* dynamic cache */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of elements stored (a(1)..a(cache_size)) */
static size_t cache_cap  = 0;   /* allocated capacity */

/* enlarge cache to accommodate at least 'need' elements */
static void grow_cache(size_t need)
{
    if (need <= cache_cap) return;

    size_t new_cap = cache_cap ? cache_cap : 32;
    while (new_cap < need) new_cap <<= 1;

    seq_t *newp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!newp) exit(1);                /* memory exhaustion */

    if (cache) {
        size_t i;
        for (i = 0; i < cache_size; ++i)
            newp[i] = cache[i];
        free(cache);
    }
    cache = newp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;

    /* quick return for first 21 terms */
    if (n <= 21) return base[(size_t)n];

    /* initialise cache on first use */
    if (!cache) {
        grow_cache(32);
        size_t i;
        for (i = 0; i < 21; ++i) cache[i] = base[i + 1];
        cache_size = 21;
    }

    /* compute missing terms up to n */
    while (cache_size < (size_t)n) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;
        /* compute a(cache_size+1) */
        size_t i;
        for (i = 0; i < 20; ++i) {
            size_t idx = cache_size - 1 - i;  /* a(n-1-i) */
            seq_t val = cache[idx];
            long long c = coeff[i];
            if (c >= 0) pos_sum += (seq_t)c * val;
            else        neg_sum += (seq_t)(-c) * val;
        }
        seq_t next = pos_sum - neg_sum;   /* pos_sum >= neg_sum always */

        grow_cache(cache_size + 1);
        cache[cache_size] = next;
        cache_size += 1;
    }

    return cache[(size_t)n - 1];
}
