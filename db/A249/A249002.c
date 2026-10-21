/*
 * A249002  Number of length 1+4 0..n arrays with no five consecutive terms
 *          having two times the sum of any three elements equal to three
 *          times the sum of the remaining two.
 * https://oeis.org/A249002
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 3*a(n-2) - a(n-3) - 5*a(n-4) - 3*a(n-5) +
 *            6*a(n-6) + 6*a(n-7) - 3*a(n-8) - 5*a(n-9) - a(n-10) + 3*a(n-11)
 *            + a(n-12) - a(n-13) Initial values (a(1) .. a(13)): 30, 190,
 *            820, 2540, 6450, 13990, 27740, 50260, 86030, 139450, 217320,
 *            325940, 475630
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 83, u64 n <= 209, u128 n <= 209,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* ------------------------------------------------------------
   Memoized linear‑recurrence implementation
   ------------------------------------------------------------ */

static seq_t *cache      = NULL; /* array of computed terms  */
static idx_t  cache_cnt  = 0;    /* number of valid elements    */
static idx_t  cache_size = 0;    /* allocated size (elements)   */

/* Ensure that cache has room for at least n+1 elements
   (indices 0 … n).  Allocate or grow the array on demand.  */
static void
grow_cache(idx_t n)
{
    if (cache_size <= n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* realloc may return NULL only when size is zero; we
           avoid that by always allocating at least 14.         */
        cache = tmp;
        cache_size = new_size;
    }
}

/* Initialise the cache with the first 13 values.  New
   terms are computed lazily by the recurrence.  */
static void
init_cache(void)
{
    if (cache_size == 0) {
        /* We need room for 0…13 indices (14 slots). */
        size_t init_size = 14;
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        cache_size = init_size;
        /* Zero all entries first. */
        for (idx_t i = 0; i < init_size; ++i)
            cache[i] = (seq_t)0;

        /* Fill a(1)..a(13).  a(0) stays zero. */
        cache[1] = (seq_t)30ULL;
        cache[2] = (seq_t)190ULL;
        cache[3] = (seq_t)820ULL;
        cache[4] = (seq_t)2540ULL;
        cache[5] = (seq_t)6450ULL;
        cache[6] = (seq_t)13990ULL;
        cache[7] = (seq_t)27740ULL;
        cache[8] = (seq_t)50260ULL;
        cache[9] = (seq_t)86030ULL;
        cache[10] = (seq_t)139450ULL;
        cache[11] = (seq_t)217320ULL;
        cache[12] = (seq_t)325940ULL;
        cache[13] = (seq_t)475630ULL;
        cache_cnt = 13; /* highest index computed so far */
    }
}

/* ------------------------------------------------------------
   Public API
   ------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    if (n == 0)   /* behavior for n < 1 is irrelevant */
        return (seq_t)0;

    init_cache();

    if (n <= cache_cnt)
        return cache[n];

    /* Compute missing terms up to n */
    grow_cache(n);

    for (idx_t i = cache_cnt + 1; i <= n; ++i) {
        seq_t r = cache[i - 1];
        r += (cache[i - 2] * (seq_t)3ULL);
        r -= cache[i - 3];
        r -= (cache[i - 4] * (seq_t)5ULL);
        r -= (cache[i - 5] * (seq_t)3ULL);
        r += (cache[i - 6] * (seq_t)6ULL);
        r += (cache[i - 7] * (seq_t)6ULL);
        r -= (cache[i - 8] * (seq_t)3ULL);
        r -= (cache[i - 9] * (seq_t)5ULL);
        r -= cache[i - 10];
        r += (cache[i - 11] * (seq_t)3ULL);
        r += cache[i - 12];
        r -= cache[i - 13];
        cache[i] = r;
    }
    cache_cnt = n;
    return cache[n];
}
