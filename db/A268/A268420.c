/*
 * A268420  Number of nX2 0..3 arrays with every repeated value in every row
 *          unequal to the previous repeated value, and in every column equal
 *          to the previous repeated value, and new values introduced in
 *          row-major sequential order.
 * https://oeis.org/A268420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   recurrence of order 11 a(n) = 33 a(n-1) - 389 a(n-2) + 1865
 *            a(n-3) - 2461 a(n-4) - 3639 a(n-5) + 7361 a(n-6) + 2407 a(n-7) -
 *            5491 a(n-8) - 585 a(n-9) + 981 a(n-10) - 81 a(n-11) Initial
 *            values: a(1)=2, a(2)=15, a(3)=187, a(4)=2530, a(5)=34195,
 *            a(6)=448667, a(7)=5720450, a(8)=71224847, a(9)=8699699...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static const seq_t init[12] = {
    0ULL,              // a(0) unused
    2ULL,              // a(1)
    15ULL,             // a(2)
    187ULL,            // a(3)
    2530ULL,           // a(4)
    34195ULL,          // a(5)
    448667ULL,         // a(6)
    5720450ULL,        // a(7)
    71224847ULL,       // a(8)
    869969995ULL,      // a(9)
    10461568690ULL,    // a(10)
    124193817155ULL    // a(11)
};

static seq_t *cache = NULL;      // 1‑based array of computed terms
static idx_t cache_size = 0;     // current maximum computed n

/* ensure cache holds terms up to n */
static void ensure_cache(idx_t n)
{
    if (n <= cache_size) return;

    /* allocate growth (double) to amortize realloc cost */
    idx_t new_size = n + 10;            /* at least n + 10 */
    while (new_size < cache_size * 2) new_size *= 2;

    seq_t *new_cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    if (!new_cache) {
        /* out of memory – abort */
        return;
    }
    cache = new_cache;

    /* initialize newly allocated entries beyond current size */
    for (idx_t i = cache_size + 1; i <= new_size; ++i)
        cache[i] = 0;

    /* copy initial values into cache if not already present */
    for (idx_t i = 1; i <= 11 && i <= new_size; ++i)
        cache[i] = init[i];

    cache_size = new_size;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n < 1, return 0 */

    ensure_cache(n);

    /* If value already computed, return it */
    if (n <= 11) return cache[n];

    /* compute missing terms up to n */
    for (idx_t k = 12; k <= n; ++k) {
        /* combine positive coefficient terms first */
        seq_t pos = 0;
        pos += 33ULL * cache[k - 1];
        pos += 1865ULL * cache[k - 3];
        pos += 7361ULL * cache[k - 6];
        pos += 2407ULL * cache[k - 7];
        pos += 981ULL * cache[k - 10];

        /* subtract negative coefficient terms */
        seq_t neg = 0;
        neg += 389ULL * cache[k - 2];
        neg += 2461ULL * cache[k - 4];
        neg += 3639ULL * cache[k - 5];
        neg += 5491ULL * cache[k - 8];
        neg += 585ULL * cache[k - 9];
        neg += 81ULL * cache[k - 11];

        cache[k] = pos - neg;   /* safe: pos > neg for all k */
    }

    return cache[n];
}
