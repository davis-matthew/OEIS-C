/*
 * A282787  Number of n X 4 0..1 arrays with no 1 equal to more than one of
 *          its king-move neighbors, with the exception of exactly one
 *          element.
 * https://oeis.org/A282787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 28*a(n-2) + 26*a(n-3) - 404*a(n-4) -
 *            1508*a(n-5) - 2631*a(n-6) - 1242*a(n-7) + 1650*a(n-8) +
 *            5880*a(n-9) + 5486*a(n-10) + 2762*a(n-11) - 2635*a(n-12) -
 *            3886*a(n-13) - 3543*a(n-14) - 896*a(n-15) - 39*a(n-16) +
 *            322*a(n-17) - 49*a(n-18), with a(1..18) given below
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[18] = {
    2ULL, 16ULL, 318ULL, 1952ULL, 16584ULL, 119176ULL, 832218ULL,
    5780340ULL, 39020884ULL, 260919192ULL, 1725189008ULL,
    11301829056ULL, 73518360532ULL, 475188725292ULL,
    3055000301306ULL, 19549420762100ULL, 124588203699132ULL,
    791140457595836ULL
};

static seq_t *cache = NULL;          /* stores a[0..max]  (a[0] unused) */
static size_t cache_cap = 0;         /* allocated capacity */
static size_t cached_max = 0;        /* highest index that has been computed */

/* enlarge the cache to keep at least n+1 elements (indices 0..n) */
static void grow_cache(idx_t n)
{
    size_t new_cap = (size_t)n + 1;
    if (new_cap <= cache_cap)
        return;

    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();

    /* initialise new elements to zero */
    for (size_t i = cache_cap; i < new_cap; ++i)
        new_cache[i] = (seq_t)0;

    cache = new_cache;
    cache_cap = new_cap;
}

/* fill cache up to index n (inclusive) if needed */
static void ensure_cache(idx_t n)
{
    if ((size_t)n <= cached_max)
        return;                     /* already computed */

    grow_cache(n);

    /* initialise the first 18 terms if they have not been set yet */
    if (cached_max < 18) {
        size_t limit = n < 18 ? n : 18;
        for (size_t i = 1; i <= limit; ++i)
            cache[i] = init_vals[i - 1];
    }

    /* nothing more to do when n <= 18 */
    if ((size_t)n <= 18) {
        cached_max = (size_t)n;
        return;
    }

    /* compute subsequent terms using the recurrence */
    size_t start = cached_max > 18 ? cached_max : 18;
    for (size_t i = start + 1; i <= (size_t)n; ++i) {
        seq_t sum = 0;
        sum += (seq_t)4   * cache[i - 1];
        sum += (seq_t)28  * cache[i - 2];
        sum += (seq_t)26  * cache[i - 3];
        sum -= (seq_t)404 * cache[i - 4];
        sum -= (seq_t)1508 * cache[i - 5];
        sum -= (seq_t)2631 * cache[i - 6];
        sum -= (seq_t)1242 * cache[i - 7];
        sum += (seq_t)1650 * cache[i - 8];
        sum += (seq_t)5880 * cache[i - 9];
        sum += (seq_t)5486 * cache[i - 10];
        sum += (seq_t)2762 * cache[i - 11];
        sum -= (seq_t)2635 * cache[i - 12];
        sum -= (seq_t)3886 * cache[i - 13];
        sum -= (seq_t)3543 * cache[i - 14];
        sum -= (seq_t)896  * cache[i - 15];
        sum -= (seq_t)39   * cache[i - 16];
        sum += (seq_t)322  * cache[i - 17];
        sum -= (seq_t)49   * cache[i - 18];
        cache[i] = sum;
    }

    cached_max = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;               /* undefined for n < 1 */

    ensure_cache(n);
    return cache[n];
}
