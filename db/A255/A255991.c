/*
 * A255991  Number of length n+7 0..1 arrays with at most one downstep in
 *          every 7 consecutive neighbor pairs.
 * https://oeis.org/A255991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 41, u64 n <= 91, u128 n <= 191,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A255991: Number of length n+7 0..1 arrays with at most one downstep in every 7 consecutive neighbor pairs.
   Recurrence (empirical): a(n) = 2*a(n-1) - a(n-2) + 6*a(n-7) - 5*a(n-8)
   Initial values: a(1)=93,a(2)=136,a(3)=207,a(4)=328,a(5)=530,a(6)=854,a(7)=1352,a(8)=2088. */
#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    /* Maximum supported index – sufficient for the intended range (≈10 000). */
    enum { MAX_N = 12000 };               /* must be >10000 */

    /* Static arrays: cache of computed values and a flag for initialisation. */
    static seq_t cache[MAX_N + 1];
    static idx_t max_computed = 0;        /* highest index that has a defined value */
    static int  initialised   = 0;        /* flag for first call */

    /* Initialise base values once. */
    if (!initialised) {
        cache[0] = (seq_t)0;             /* unused but convenient */
        cache[1] = (seq_t)93;
        cache[2] = (seq_t)136;
        cache[3] = (seq_t)207;
        cache[4] = (seq_t)328;
        cache[5] = (seq_t)530;
        cache[6] = (seq_t)854;
        cache[7] = (seq_t)1352;
        cache[8] = (seq_t)2088;
        max_computed = 8;
        initialised = 1;
    }

    /* If the requested term is already known, return it. */
    if (n <= max_computed && n <= MAX_N)
        return cache[n];

    /* If n is too large for our static array, fall back to 0 (should not happen for the intended use). */
    if (n > MAX_N)
        return (seq_t)0;

    /* Compute missing terms up to n. */
    for (idx_t k = max_computed + 1; k <= n; ++k) {
        /* Recurrence is defined for k >= 9; for k <= 8 we already have base values. */
        cache[k] = ( seq_t )2  * cache[k-1]    /* 2*a(n-1) */
                 -            cache[k-2]     /* - a(n-2) */
                 + (seq_t)6  * cache[k-7]    /* + 6*a(n-7) */
                 - (seq_t)5  * cache[k-8];   /* - 5*a(n-8) */
    }
    max_computed = n;
    return cache[n];
}
