/*
 * A350439  a(n) is the number of integers that can be represented in a
 *          7-segment display by using only n segments (version A063720).
 * https://oeis.org/A350439
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n-2)+a(n-3)+a(n-4)+5*a(n-5)+a(n-6)+a(n-7) for n>13
 *            Initial values:
 *            a(0)=0,a(1)=0,a(2)=1,a(3)=2,a(4)=3,a(5)=9,a(6)=12,a(7)=20,
 *            a(8)=35,a(9)=58,a(10)=116,a(11)=180,a(12)=329,a(13)=560
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache   = NULL;   /* array of already computed values   */
static idx_t  cap     = 0;       /* allocated size of cache            */
static idx_t  known   = 13;      /* largest index that has been filled */

/* Fill the first 14 values of the cache.  Must be called only when
   cache was NULL. */
static void init_cache(void)
{
    static const seq_t init_vals[14] = {
        0ULL, 0ULL, 1ULL, 2ULL, 3ULL, 9ULL, 12ULL, 20ULL,
        35ULL, 58ULL, 116ULL, 180ULL, 329ULL, 560ULL
    };
    /* allocate space for at least 14 elements.  If a larger n is
       requested later, ensure_cache will enlarge it. */
    cap = 14;
    cache = (seq_t *)malloc(cap * sizeof(seq_t));
    if (!cache) exit(1);
    for (idx_t i = 0; i < 14; ++i) cache[i] = init_vals[i];
}

/* Ensure that cache is allocated and large enough to hold indices
   up to n (inclusive).  If it was NULL, it will be initialized. */
static void ensure_cache(idx_t n)
{
    if (!cache)
    {
        init_cache();
        return;
    }
    if (n + 1 > cap)
    {
        idx_t new_cap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    /* Make sure the cache exists before any access. */
    ensure_cache(n);

    /* For known small indices, the result is already in the cache. */
    if (n <= known) return cache[n];

    /* Compute all missing values up to n.  The recurrence never
       accesses indices < 0 because we start at known+1 (>=14). */
    for (idx_t i = known + 1; i <= n; ++i)
    {
        cache[i] = cache[i - 2] + cache[i - 3] + cache[i - 4]
                 + 5 * cache[i - 5] + cache[i - 6] + cache[i - 7];
    }
    known = n;
    return cache[n];
}
