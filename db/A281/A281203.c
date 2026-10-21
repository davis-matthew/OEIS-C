/*
 * A281203  Number of n X 6 0..1 arrays with no element equal to more than one
 *          of its horizontal and antidiagonal neighbors, with the exception
 *          of exactly one element, and with new values introduced in order 0
 *          sequentially upwards.
 * https://oeis.org/A281203
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 3*a(n-2) - 4*a(n-3) - 6*a(n-4) + 4*a(n-6) +
 *            6*a(n-7) + 3*a(n-8) - 2*a(n-9) - 3*a(n-10) - 2*a(n-11) -
 *            a(n-12). Initial values: a(1)=10, a(2)=94, a(3)=310, a(4)=804,
 *            a(5)=1906, a(6)=4248, a(7)=9118, a(8)=19026, a(9)=38916,
 *            a(10)=78356, a(11)=155834, a(12)=306840. The recurrence is...
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 26, u64 n <= 63, u128 n <= 138,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Cache for computed values.  cache[0] holds a(1). */
static seq_t *cache = NULL;
static size_t cache_cap = 0;

/* Ensure that cache can hold values up to index n-1.  The cache is
   grown geometrically to avoid excessive reallocations.  Newly
   allocated entries are zero-initialised and thus indicate “not
   yet computed”. */
static void ensure_cache(idx_t n)
{
    size_t needed = (size_t)n;
    if (cache && cache_cap >= needed)
        return;

    size_t new_cap = cache_cap ? cache_cap : 64;
    while (new_cap < needed)
        new_cap <<= 1;         /* double until large enough */

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        exit(EXIT_FAILURE);   /* out of memory */

    /* Zero new allocations. */
    for (size_t i = cache_cap; i < new_cap; ++i)
        tmp[i] = 0;

    cache = tmp;
    cache_cap = new_cap;
}

/* Coefficients of the recurrence, indexed by k=1..12. */
static const int coeff[12] = {
    2,  3, -4, -6, 0, 4, 6, 3, -2, -3, -2, -1
};

/* Initial terms a(1)..a(12).  Index 0 holds a(1). */
static const seq_t init[12] = {
    10ULL, 94ULL, 310ULL, 804ULL, 1906ULL, 4248ULL,
    9118ULL, 19026ULL, 38916ULL, 78356ULL,
    155834ULL, 306840ULL
};

/* Compute a(n).  The function is fully memoised and handles calls
   in any order. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n<1 */

    ensure_cache(n);

    /* Initialise the first 12 values once. */
    if (cache[0] == 0) {
        for (idx_t i = 1; i <= 12; ++i)
            cache[i - 1] = init[i - 1];
    }

    if (n <= 12)
        return cache[n - 1];

    /* Compute values 13 .. n on demand. */
    for (idx_t i = 13; i <= n; ++i) {
        if (cache[i - 1] != 0)
            continue;

        seq_t pos_sum = 0, neg_sum = 0;
        for (int k = 1; k <= 12; ++k) {
            idx_t idx = i - k;
            seq_t term = cache[idx - 1];
            int c     = coeff[k - 1];

            if (c > 0)
                pos_sum += (seq_t)c * term;
            else if (c < 0)
                neg_sum += (seq_t)(-c) * term;
        }
        cache[i - 1] = pos_sum - neg_sum;
    }

    return cache[n - 1];
}
