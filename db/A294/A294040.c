/*
 * A294040  a(n) = e*(Gamma(2*n,1) - Gamma(n,1)).
 * https://oeis.org/A294040
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = e*(Gamma(2*n,1) - Gamma(n,1))
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*          a(n) = A000522(2*n-1) - A000522(n-1) */
/*          where A000522(m) = sum_{k=0..m} m! / k! */
/*          Recurrence: A000522(m) = m * A000522(m-1) + 1,  A000522(0) = 1 */

#include <stdlib.h>
#include <stddef.h>

/* Static cache for A000522 values */
static seq_t *s_cache = NULL;   /* pointer to cached values */
static idx_t s_size  = 0;       /* number of values currently computed (max index + 1) */
static size_t s_cap  = 0;       /* allocated capacity of cache */

/* Ensure cache has space for indices up to 'needed' (inclusive) */
static void ensure_cache(idx_t needed)
{
    if (needed + 1 <= s_cap)
        return;

    size_t new_cap = (needed + 1) * 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_cache)
        exit(EXIT_FAILURE);

    /* copy old values */
    for (idx_t i = 0; i < s_size; ++i)
        new_cache[i] = s_cache[i];

    /* free old cache if it existed */
    free(s_cache);
    s_cache = new_cache;
    s_cap   = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* required maximum index for A000522 */
    idx_t required = 2 * n - 1;          /* 2*n-1 is safe for n>=1 */

    /* extend cache if necessary */
    ensure_cache(required);

    /* compute any missing A000522 values */
    for (idx_t i = s_size; i <= required; ++i) {
        if (i == 0)
            s_cache[0] = 1;                        /* A000522(0) = 1 */
        else
            s_cache[i] = (seq_t)i * s_cache[i - 1] + 1;  /* recurrence */
    }
    s_size = required + 1;

    /* a(n) = A000522(2*n-1) - A000522(n-1) */
    seq_t result = s_cache[required] - s_cache[n - 1];
    return result;
}
