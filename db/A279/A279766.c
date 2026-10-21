/*
 * A279766  Number of odd digits in the decimal expansions of integers 1 to n.
 * https://oeis.org/A279766
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of odd decimal digits in the decimal expansions of
 *            1..n
 * Range:     u8 n <= 177, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Helper to count odd decimal digits of an idx_t integer */
static inline seq_t count_odd_digits(idx_t x)
{
    seq_t odd = 0;
    while (x != 0) {
        unsigned d = x % 10ULL;
        if (d & 1U) odd++;
        x /= 10ULL;
    }
    return odd;
}

seq_t a(idx_t n)
{
    /* static cache: cache[i] = a(i) for 0 <= i <= max_cached */
    static seq_t *cache = NULL;
    static idx_t max_cached = 0;
    static size_t cap = 0;

    /* Initialize cache on first call */
    if (cache == NULL) {
        cap = 1024;                  /* initial capacity (power of two) */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure: return 0 */
        cache[0] = (seq_t)0;
        max_cached = 0;
    }

    /* Extend cache if needed */
    if (n > max_cached) {
        /* Grow capacity to at least n+1 */
        while (cap <= n) cap <<= 1;
        cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */
        /* Compute new values */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t odd = count_odd_digits(i);
            cache[(size_t)i] = cache[(size_t)i - 1] + odd;
        }
        max_cached = n;
    }

    return cache[(size_t)n];
}
