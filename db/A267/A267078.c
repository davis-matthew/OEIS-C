/*
 * A267078  Coefficient of x^0 in the minimal polynomial of the continued
 *          fraction [1^n,2^(1/3),1,1,...], where 1^n means n ones.
 * https://oeis.org/A267078
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 13*a(n-1) + 104*a(n-2) - 260*a(n-3) - 260*a(n-4) +
 *            104*a(n-5) + 13*a(n-6) - a(n-7) for n > 8 Initial values: a(0) =
 *            -5, a(1) = -11, a(2) = 131, a(3) = 3421, a(4) = 56209, a(5) =
 *            1049105, a(6) = 18561659, a(7) = 334918459, a(8) = 5997328339
 * Range:     i8 n <= 1, i16 n <= 3, i32 n <= 7, i64 n <= 15, i128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array of computed terms */
static size_t cache_len = 0;         /* number of valid entries in cache */
static size_t cache_cap = 0;         /* capacity of allocated array */

/* Ensure the cache can hold at least indices 0..n.  The capacity is
   rounded up to a power of two for amortised O(1) growth. */
static void ensure_cache_cap(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (cache_cap >= need)
        return;
    size_t new_cap = cache_cap > 0 ? cache_cap : 1;
    while (new_cap < need)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first nine terms of the sequence.  This is called
   lazily the first time 'a' is invoked. */
static void init_cache(void)
{
    if (cache_len > 0)         /* already initialised */
        return;

    ensure_cache_cap(8);       /* need indices 0..8 */

    cache[0] = (seq_t)(-5);
    cache[1] = (seq_t)(-11);
    cache[2] = (seq_t)(131);
    cache[3] = (seq_t)(3421);
    cache[4] = (seq_t)(56209);
    cache[5] = (seq_t)(1049105);
    cache[6] = (seq_t)(18561659);
    cache[7] = (seq_t)(334918459);
    cache[8] = (seq_t)(5997328339);

    cache_len = 9;             /* 0 .. 8 valid */
}

/* Compute a(idx_t n).  The function is safe for arbitrary order of
   calls and uses memoisation to avoid recomputation. */
seq_t a(idx_t n)
{
    init_cache();

    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Ensure capacity for up to index n. */
    ensure_cache_cap(n);

    /* Compute missing values starting from the current cache_len. */
    for (size_t i = cache_len; i <= (size_t)n; ++i)
    {
        seq_t res = cache[i-1] * (seq_t)13
                  + cache[i-2] * (seq_t)104
                  - cache[i-3] * (seq_t)260
                  - cache[i-4] * (seq_t)260
                  + cache[i-5] * (seq_t)104
                  + cache[i-6] * (seq_t)13
                  - cache[i-7];
        cache[i] = res;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
