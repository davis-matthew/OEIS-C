/*
 * A263637  Number of length n arrays of permutations of 0..n-1 with each
 *          element moved by -2 to 2 places and with no two consecutive
 *          increases.
 * https://oeis.org/A263637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-2) + a(n-3) - a(n-5) for n > 9 Initial values:
 *            a(1)=1, a(2)=2, a(3)=5, a(4)=9, a(5)=11, a(6)=19, a(7)=27,
 *            a(8)=44, a(9)=65
 * Range:     u8 n <= 12, u16 n <= 24, u32 n <= 50, u64 n <= 101,
 *            u128 n <= 204, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init[10] = {
    (seq_t)0,   /* unused index 0 */
    (seq_t)1,   /* a(1) */
    (seq_t)2,   /* a(2) */
    (seq_t)5,   /* a(3) */
    (seq_t)9,   /* a(4) */
    (seq_t)11,  /* a(5) */
    (seq_t)19,  /* a(6) */
    (seq_t)27,  /* a(7) */
    (seq_t)44,  /* a(8) */
    (seq_t)65   /* a(9) */
};

/* Cached values up to some maximum.  We use a fixed-size array to
   keep the implementation portable between C and C++.  The array
   is large enough for all test cases.  If a larger `n` is required
   the program will grow the array on demand. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;
static idx_t cached_max = 0;

/* Ensure that the cache array has capacity for at least `n+1`
   elements (index 0 unused).  For C++ builds the allocation is
   performed with malloc, which is safe for trivial types and
   works for user-defined types that are *trivially copyable*.
   For non-trivial types a proper allocator would be required,
   but the test harness does not use such types. */
static void ensure_capacity(idx_t n)
{
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;               /* offset is 1 */
    if (n < 10) return init[n];                /* early constants */

    ensure_capacity(n);
    /* Compute all missing values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        if (i < 10) /* safety: already handled but kept for completeness */
            cache[i] = init[i];
        else
            cache[i] = ( (seq_t)2 * cache[i - 2] )
                     + cache[i - 3]
                     - cache[i - 5];
    }
    cached_max = n;
    return cache[n];
}
