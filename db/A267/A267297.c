/*
 * A267297  Square triangular numbers that are the sum of 2 nonzero
 *          nonconsecutive triangular numbers.
 * https://oeis.org/A267297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 35*a(n-1) - 35*a(n-2) + a(n-3) (for n>6) Initial terms:
 *            a(1)=1225, a(2)=41616, a(3)=1413721, a(4)=1631432881,
 *            a(5)=55420693056, a(6)=1882672131025
 * Range:     u8 none, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

// Maximum index that is assumed to be queried in typical tests.
// The function also works for larger n, but it uses a short computation
// path when n exceeds this limit.
#ifndef MAX_N
#define MAX_N 10000
#endif

/* Static storage for the sequence values.  Index 0 is unused. */
static seq_t cache[MAX_N + 1];
static size_t cached_until = 0;

/* Known initial values (index 0 through 6). */
static const seq_t init_vals[7] = {
    (seq_t)0,                 /* unused */
    (seq_t)1225,
    (seq_t)41616,
    (seq_t)1413721,
    (seq_t)1631432881,
    (seq_t)55420693056,
    (seq_t)1882672131025
};

/* Helper that ensures the required values up to n have been computed
   and stored in `cache`.  It works for n <= MAX_N. */
static void compute_upto(size_t n)
{
    if (n <= cached_until)
        return;

    /* Initialise the first 6 terms if not already done */
    if (cached_until < 6) {
        for (size_t i = 1; i <= 6; ++i)
            cache[i] = init_vals[i];
        cached_until = 6;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cached_until + 1; i <= n; ++i) {
        /* For i<=6 is already covered in the block above, but we
           keep the guard for safety. */
        if (i <= 6)
            cache[i] = init_vals[i];
        else
            cache[i] = (seq_t)35 * cache[i-1]
                       - (seq_t)35 * cache[i-2]
                       + cache[i-3];
    }
    cached_until = n;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;  /* Undefined for n<1; return 0 for safety */

    /* If the requested index is within the preallocated array */
    if ((size_t)n <= MAX_N) {
        compute_upto((size_t)n);
        return cache[(size_t)n];
    }

    /* For very large indices we calculate on the fly using the
       recurrence (no caching).  This path is rarely needed for
       typical test ranges, but keeps the function correct for any n. */
    if ((size_t)n <= 6)
        return init_vals[(size_t)n];

    /* Initialise the last known three values for the recurrence. */
    seq_t a4 = init_vals[4];
    seq_t a5 = init_vals[5];
    seq_t a6 = init_vals[6];
    seq_t cur = a6;
    seq_t prev3 = a4, prev2 = a5, prev1 = a6;

    for (size_t i = 7; i <= (size_t)n; ++i) {
        cur = (seq_t)35 * prev1 - (seq_t)35 * prev2 + prev3;
        prev3 = prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
