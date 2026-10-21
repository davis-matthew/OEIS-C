/*
 * A276600  Values of m such that m^2 + 6 is a triangular number (A000217).
 * https://oeis.org/A276600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=0, a(2)=2, a(3)=3, a(4)=7, a(5)=15, a(6)=20; for n>6: a(n)
 *            = 6*a(n-3) - a(n-6)
 * Range:     u8 n <= 10, u16 n <= 19, u32 n <= 38, u64 n <= 76, u128 n <= 151,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: the sequence is indexed from 1.  The function accepts
   any n >= 1 and internally memoises all values up to the largest
   requested n.  A fixed-size cache of 10010 elements is sufficient
   for the required range (up to n≈10000).  The same code
   compiles under C11 and C++17 because all operations are purely
   integer-based. */

#include <stdlib.h>

static const seq_t init_vals[6] = {0, 2, 3, 7, 15, 20};
#if 0
/* In C mode the size of seq_t is an unsigned integer; in C++ mode
   it may be an arbitrary-precision class (e.g. boost::multiprecision::cpp_int).
   The following static array holds values up to n = 10000. */
#endif
static seq_t cache[10010];                 /* indices 1 .. 10009 used */
static size_t max_cached = 0;              /* maximum n that has been computed */

/* Ensure the cache contains values up to n.  The routine grows the
   cache only if necessary (within the fixed-size limit). */
static void
ensure_cached(idx_t n)
{
    if (n <= max_cached)
        return;                            /* already computed */

    /* Initialise first six values if this is the first call. */
    if (max_cached == 0) {
        for (idx_t i = 0; i < 6; ++i)
            cache[i + 1] = init_vals[i];
        max_cached = 6;
    }

    /* Compute missing values using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* a(i) = 6*a(i-3) - a(i-6)  -> indices i-3, i-6 */
        seq_t val = 6 * cache[i - 3] - cache[i - 6];
        cache[i] = val;
    }
    max_cached = n;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                          /* safety for out of bounds */

    if (n < 7)                              /* direct access to initial terms */
        return init_vals[n - 1];

    /* Ensure cache has the value up to n */
    ensure_cached(n);
    return cache[n];
}
