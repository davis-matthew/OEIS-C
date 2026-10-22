/*
 * A302676  Number of n X 4 0..1 arrays with every element equal to 0, 1, 4 or
 *          6 horizontally, diagonally or antidiagonally adjacent elements,
 *          with upper left element zero.
 * https://oeis.org/A302676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+2*a(n-2)-a(n-4) for n > 8; Initial values: a(1)=5,
 *            a(2)=5, a(3)=12, a(4)=20, a(5)=33, a(6)=64, a(7)=121, a(8)=231
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 68, u128 n <= 137,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache already computed terms up to the largest index seen.
       The cache grows only, so repeated calls with arbitrary
       ordering of arguments are cheap and correct. */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* current capacity of cache */
    static size_t max_n = 0;   /* largest n for which cache[i-1] is valid */

    if (n < 1) return (seq_t)0;   /* undefined region – return 0 */

    /* Ensure the cache can hold a(n).  Double the size until it fits. */
    if (cache == NULL) {
        cap   = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }
    while (cap < n) {
        size_t new_cap = cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initial values for a(1)…a(8) */
    static const seq_t init[8] = {
        (seq_t)5, (seq_t)5, (seq_t)12, (seq_t)20,
        (seq_t)33, (seq_t)64, (seq_t)121, (seq_t)231
    };

    /* Compute any missing values up to n. */
    for (size_t i = max_n + 1; i <= n; ++i) {
        if (i <= 8) {
            cache[i - 1] = init[i - 1];
        } else {
            seq_t a1 = cache[i - 2]; /* a(i-1) */
            seq_t a2 = cache[i - 3]; /* a(i-2) */
            seq_t a4 = cache[i - 5]; /* a(i-4) */
            cache[i - 1] = a1 + ((seq_t)2 * a2) - a4;
        }
    }

    max_n = (size_t)n;
    return cache[n - 1];
}
