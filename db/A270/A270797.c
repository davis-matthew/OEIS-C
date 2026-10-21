/*
 * A270797  a(n) = J(n) if n odd, or 4*J(n) if n even, where J = Jacobsthal
 *          numbers A001045.
 * https://oeis.org/A270797
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4) for n>3; a(0)=0, a(1)=1, a(2)=4,
 *            a(3)=3
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized sequence values */
static idx_t cache_size = 0;     /* number of elements currently stored */

seq_t a(idx_t n)
{
    static const int base_vals[4] = {0, 1, 4, 3};

    /* initialise cache with base terms on first call */
    if (cache_size == 0) {
        idx_t init = 4;
        cache = (seq_t *)malloc(init * sizeof(seq_t));
        if (!cache)
            return 0;                   /* allocation failure – unlikely in tests */
        for (idx_t i = 0; i < init; ++i)
            cache[i] = (seq_t)base_vals[i];
        cache_size = init;
    }

    /* handle indices less than 4 */
    if (n < 4)
        return (seq_t)base_vals[(size_t)n];

    /* grow cache if necessary and compute missing values */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp)
            return 0;                   /* allocation failure */
        cache = tmp;

        for (idx_t i = cache_size; i <= n; ++i) {
            /* Use sequence arithmetic; multiplication by small constants
               is handled by the seq_t type (it supports 5*value and 4*value). */
            cache[i] = 5 * cache[i - 2] - 4 * cache[i - 4];
        }
        cache_size = new_size;
    }

    return cache[n];
}
