/*
 * A377715  Number of permutations (p(1),p(2),...,p(n)) of (1,2,...,n) such
 *          that p(i)-i is in {-2,3,4} for all i=1,...,n.
 * https://oeis.org/A377715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) =
 *            a(n-3)+2*a(n-5)+2*a(n-6)-a(n-8)-2*a(n-9)-a(n-10)-a(n-11)-a(n-12)+a(n-15)
 *            for n ≥ 15. Initial values: a(0)=1, a(1)=0, a(2)=0, a(3)=0,
 *            a(4)=0, a(5)=1, a(6)=1, a(7)=0, a(8)=0, a(9)=0, a(10)=1,
 *            a(11)=3, a(12)=1, a(13)=0, a(14)=0, a(15)=1. This code memoises
 *            results so the function works reliab...
 * Range:     u8 n <= 37, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t base_vals[16] = {
    (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)0,
    (seq_t)1, (seq_t)3, (seq_t)1, (seq_t)0, (seq_t)0, (seq_t)1
};

static seq_t *cache = NULL;      /* memoised values a(i) */
static idx_t  cache_cap = 0;     /* number of indices allocated */
static idx_t  cache_max = 0;     /* largest n already computed */

/* Ensure that cache can store indices up to n inclusive.  The buffer only grows. */
static void ensure_cache(idx_t n)
{
    if (n < cache_cap)
        return;
    idx_t new_cap = n + 1;
    seq_t *new_buf = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_buf;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n < 16)               /* return hard‑coded initials */
        return base_vals[n];

    /* initialise cache on first call */
    if (cache == NULL) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = base_vals[i];
        cache_max = 15;
    }

    /* compute missing values up to n using the recurrence */
    if (n > cache_max) {
        ensure_cache(n);
        for (idx_t k = cache_max + 1; k <= n; ++k) {
            seq_t val = cache[k - 3];
            val += (seq_t)2 * cache[k - 5];
            val += (seq_t)2 * cache[k - 6];
            val -= cache[k - 8];
            val -= (seq_t)2 * cache[k - 9];
            val -= cache[k - 10];
            val -= cache[k - 11];
            val -= cache[k - 12];
            val += cache[k - 15];
            cache[k] = val;
        }
        cache_max = n;
    }

    return cache[n];
}
