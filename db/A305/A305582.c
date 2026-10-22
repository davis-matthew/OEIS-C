/*
 * A305582  Number of nX4 0..1 arrays with every element unequal to 0, 1, 2, 7
 *          or 8 king-move adjacent elements, with upper left element zero.
 * https://oeis.org/A305582
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-2) + 5*a(n-3) - 2*a(n-5) - 4*a(n-6) for n
 *            > 8, with initial values a(1) = 8, a(2) = 5, a(3) = 8, a(4) =
 *            15, a(5) = 38, a(6) = 102, a(7) = 247, a(8) = 617
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 49, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based storage of computed terms */
static size_t cache_cap = 0;         /* current allocated capacity (including index 0) */
static size_t cache_len = 0;         /* highest index that has been computed */

/* Ensure that the cache can hold indices up to n (inclusive). */
static void ensure_capacity(size_t n)
{
    if (cache_cap >= n) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < n) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);               /* terminate on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Return a(n) for n >= 1.  The function is memoised and works for any
   calling order of n. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;            /* undefined for n < 1 */

    /* Lazy initialization: store the first 8 values once. */
    if (cache_len == 0)
    {
        ensure_capacity(16);        /* pre‑allocate some space */
        cache[1] = (seq_t)8;
        cache[2] = (seq_t)5;
        cache[3] = (seq_t)8;
        cache[4] = (seq_t)15;
        cache[5] = (seq_t)38;
        cache[6] = (seq_t)102;
        cache[7] = (seq_t)247;
        cache[8] = (seq_t)617;
        cache_len = 8;
    }

    if ((size_t)n <= cache_len)     /* already computed */
        return cache[n];

    /* Ensure the cache is large enough for the requested index. */
    ensure_capacity((size_t)n + 1);

    /* Compute any missing values up to n. */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i)
    {
        /* recurrence: a(i) = a(i-1) + 2*a(i-2) + 5*a(i-3)
                          - 2*a(i-5) - 4*a(i-6)        */
        seq_t t = cache[i - 1];
        t += (seq_t)2 * cache[i - 2];
        t += (seq_t)5 * cache[i - 3];
        t -= (seq_t)2 * cache[i - 5];
        t -= (seq_t)4 * cache[i - 6];
        cache[i] = t;
    }

    cache_len = n;
    return cache[n];
}
