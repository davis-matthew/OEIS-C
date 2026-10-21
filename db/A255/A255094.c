/*
 * A255094  Number of (n+2) X (1+2) 0..1 arrays with no 3 x 3 subblock
 *          diagonal sum 1 and no antidiagonal sum 1 and no row sum 1 and no
 *          column sum 1.
 * https://oeis.org/A255094
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 5*a(n-3) - 2*a(n-4) + 3*a(n-5) -
 *            2*a(n-6) - 2*a(n-7) - 2*a(n-8) - 2*a(n-9) + 2*a(n-11) for n > 12
 *            Initial values: a(1) = 36, a(2) = 77, a(3) = 179, a(4) = 419,
 *            a(5) = 991, a(6) = 2345, a(7) = 5537, a(8) = 13105, a(9) =
 *            31063, a(10) = 73591, a(11) = 174311, a(12) = 412949
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 22, u64 n <= 48, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;   /* actual allocated size */
static idx_t  max_computed = 0; /* highest index that has been computed */

/* Ensure that the cache covers at least indices 0..n and that all values up to n are computed. */
static void compute_up_to(idx_t n)
{
    if (n < 1) return;

    /* Allocate initial storage (mandatory up to index 12). */
    if (!cache) {
        cache = (seq_t*)malloc(13 * sizeof(seq_t));
        cache_size = 13;
        static const seq_t init_vals[13] = {
            0ULL,                /* dummy for index 0 */
            36ULL, 77ULL, 179ULL, 419ULL, 991ULL,
            2345ULL, 5537ULL, 13105ULL, 31063ULL,
            73591ULL, 174311ULL, 412949ULL
        };
        for (idx_t i = 1; i < 13; ++i)
            cache[i] = init_vals[i];
        max_computed = 12;
    }

    /* Resize cache if necessary. */
    if (n + 1 > cache_size) {
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        cache_size = n + 1;
    }

    /* Compute missing values (only if n is larger than what we've already computed). */
    if (n > max_computed) {
        idx_t start = max_computed + 1;
        if (start < 13) start = 13;          /* recurrence only defined for n > 12 */
        for (idx_t i = start; i <= n; ++i) {
            cache[i] = 2 * cache[i - 1]
                       - cache[i - 2]
                       + 5 * cache[i - 3]
                       - 2 * cache[i - 4]
                       + 3 * cache[i - 5]
                       - 2 * cache[i - 6]
                       - 2 * cache[i - 7]
                       - 2 * cache[i - 8]
                       - 2 * cache[i - 9]
                       + 2 * cache[i - 11];
        }
        max_computed = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    compute_up_to(n);
    return cache[n];
}
