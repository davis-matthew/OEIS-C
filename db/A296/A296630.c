/*
 * A296630  Number of nX3 0..1 arrays with each 1 horizontally, vertically or
 *          antidiagonally adjacent to 0, 2 or 4 neighboring 1s.
 * https://oeis.org/A296630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 16, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A296630 – Number of n×3 0..1 arrays with the 1‑cell adjacency rule
 *  Formula:  a(n) = 2*a(n-1) +4*a(n-2) +6*a(n-3) +8*a(n-4) +6*a(n-5) +3*a(n-6)
 *             -2*a(n-7) -6*a(n-8) -8*a(n-9) -7*a(n-10) -4*a(n-11) - a(n-12)
 *
 *  Initial values:
 *    a(1)=5,   a(2)=19,  a(3)=71,  a(4)=260,  a(5)=966,  a(6)=3581,
 *    a(7)=13275, a(8)=49209, a(9)=182422, a(10)=676247, a(11)=2506882,
 *    a(12)=9293135
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t init_vals[12] = {
    5ULL,   19ULL,  71ULL,  260ULL,  966ULL,
    3581ULL,13275ULL,49209ULL,182422ULL,676247ULL,
    2506882ULL,9293135ULL
};

static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size */
static size_t cache_len = 0;   /* number of filled entries (a(1) at cache[0]) */

static void
ensure_cache(idx_t n)
{
    if (n <= cache_len) return;           /* already computed */

    /* enlarge buffer if needed */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 128;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise first 12 values if not already done */
    if (cache_len < 12) {
        size_t upto = 12;
        if (n < upto) upto = n;
        for (size_t i = cache_len; i < upto; ++i)
            cache[i] = init_vals[i];
        cache_len = (cache_len > upto ? cache_len : upto);
    }

    /* compute remaining terms using the recurrence */
    for (size_t i = cache_len; i < (size_t)n; ++i) {
        /* i corresponds to a(i+1) ; we need indices i-1 .. i-12   */
        seq_t val = 0;
        val += cache[i-1] * 2ULL;
        val += cache[i-2] * 4ULL;
        val += cache[i-3] * 6ULL;
        val += cache[i-4] * 8ULL;
        val += cache[i-5] * 6ULL;
        val += cache[i-6] * 3ULL;
        val -= cache[i-7] * 2ULL;
        val -= cache[i-8] * 6ULL;
        val -= cache[i-9] * 8ULL;
        val -= cache[i-10] * 7ULL;
        val -= cache[i-11] * 4ULL;
        val -= cache[i-12];
        cache[i] = val;
    }
    cache_len = n;
}

/*  External linkage as required. */
seq_t
a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n<1 – return 0 */
    ensure_cache(n);
    return cache[(size_t)n - 1];
}
