/*
 * A253507  Number of (n+2) X (5+2) 0..1 arrays with every 2 X 2 and 3 X 3
 *          subblock diagonal maximum minus antidiagonal minimum nondecreasing
 *          horizontally and vertically.
 * https://oeis.org/A253507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*a(n-1) - 10*a(n-2) + 10*a(n-3) - 5*a(n-4) + a(n-5)
 *            (valid for n > 13) Initial terms: a(1)=884, a(2)=1712,
 *            a(3)=3052, a(4)=5136, a(5)=7948, a(6)=11740, a(7)=16592,
 *            a(8)=22720, a(9)=30300, a(10)=39560, a(11)=50740, a(12)=64104,
 *            a(13)=79936
 * Range:     u8 none, u16 n <= 12, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Base values stored as unsigned long long to avoid problems with
   arbitrary‑precision seq_t types in C. */
static const unsigned long long base_vals_raw[13] = {
    884ULL, 1712ULL, 3052ULL, 5136ULL, 7948ULL,
    11740ULL, 16592ULL, 22720ULL, 30300ULL,
    39560ULL, 50740ULL, 64104ULL, 79936ULL
};

static seq_t *cache = NULL;    /* 1‑based cache for computed values */
static size_t cache_cap = 0;   /* highest n currently stored in cache */

/* Ensure the cache holds at least n entries (indices 0..n). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap) return;
    size_t new_cap = (size_t)n;
    seq_t *p = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
    if (!p) abort();               /* allocation failed */
    cache = p;
    cache_cap = new_cap;
}

/* Sequence function. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1 */

    size_t old_cap = cache_cap;   /* current cache size before possible growth */
    if ((size_t)n > old_cap)      /* grow the cache if needed */
        ensure_capacity(n);

    /* Compute missing values from old_cap+1 up to n. */
    for (idx_t i = old_cap + 1; i <= n; ++i)
    {
        if (i <= 13) {                 /* base cases */
            cache[i] = (seq_t)base_vals_raw[i - 1];
        } else {                       /* recurrence for n > 13 */
            cache[i] = 5 * cache[i - 1]
                     - 10 * cache[i - 2]
                     + 10 * cache[i - 3]
                     - 5 * cache[i - 4]
                     + cache[i - 5];
        }
    }

    cache_cap = n;  /* update the highest computed index */
    return cache[n];
}
