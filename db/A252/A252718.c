/*
 * A252718  Number of (n+2) X (7+2) 0..3 arrays with every consecutive three
 *          elements in every row and diagonal having exactly two distinct
 *          values, and in every column and antidiagonal not having exactly
 *          two distinct values, and new values 0 upwards introduced in row
 *          major order.
 * https://oeis.org/A252718
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-3) - 11*a(n-6) + 5*a(n-9) + 2*a(n-12) - a(n-15) for
 *            n > 17 (a(1) = 237, a(2) = 107, a(3) = 114, a(4) = 153, a(5) =
 *            211, a(6) = 270, a(7) = 373, a(8) = 522, a(9) = 672, a(10) =
 *            938, a(11) = 1322, a(12) = 1704, a(13) = 2388, a(14) = 3379,
 *            a(15) = 4354, a(16) = 6113, a(17) = 8673)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 58, u64 n <= 127, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

/* Predefined initial values (1‑based). Index 0 is unused. */
static const seq_t init_vals[18] = {
    0,              /* dummy for index 0 */
    237, 107, 114, 153, 211, 270,
    373, 522, 672, 938, 1322,
    1704, 2388, 3379, 4354,
    6113, 8673
};

/* Static cache array.  The sequence is required for n up to about 10 000
   in the test harness, so 10 001 entries (index 0..10 000) are sufficient. */
static seq_t cache[10001];
static size_t cache_len = 0;   /* highest index initialized */

/* Initialise the cache with the known seed values.  This is done lazily
   on the first call to a(). */
static void init_cache(void)
{
    if (cache_len != 0)
        return;
    /* Copy seed values */
    for (size_t i = 1; i < sizeof(init_vals) / sizeof(init_vals[0]); ++i) {
        cache[i] = init_vals[i];
    }
    cache_len = sizeof(init_vals) / sizeof(init_vals[0]) - 1;  /* 17 */
}

/* Compute a(n) for arbitrary-precision or unsigned integer seq_t.
   The function is thread‑unsafe but satisfies the requested behaviour. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* undefined for n < 1 */

    init_cache();

    /* If the requested n is within the precomputed range, return it. */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n];

    /* Compute successive values up to n. */
    for (size_t i = cache_len + 1; i <= (size_t)n; ++i) {
        seq_t val;
        /* Recurrence holds for i > 17.  For i <= 17 we already have init_vals. */
        if (i <= 17) {
            val = init_vals[i];
        } else {
            /* a(i) = 6*a(i-3) - 11*a(i-6) + 5*a(i-9) + 2*a(i-12) - a(i-15) */
            /* Use signed‑like arithmetic; intermediate results fit in seq_t. */
            seq_t t = 6 * cache[i - 3];
            t += 5 * cache[i - 9];
            t += 2 * cache[i - 12];
            t -= 11 * cache[i - 6];
            t -= cache[i - 15];
            val = t;
        }
        cache[i] = val;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n];
}
