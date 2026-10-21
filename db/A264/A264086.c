/*
 * A264086  Number of (n+1) X (4+1) arrays of permutations of 0..n*5+4 with
 *          each element having index change +-(.,.) 0,0 0,1 or 2,-2.
 * https://oeis.org/A264086
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence with initial values:
   a(1) = 64,   a(2) = 1448,   a(3) = 32761,   a(4) = 645627
   a(n) = 19*a(n-1) + 304*a(n-3) + 256*a(n-4)   for n ≥ 5
*/

#include <stdlib.h>   /* abort() */
#include <stdint.h>   /* for uint64_t if needed */
#include <stdbool.h>  /* bool */

seq_t a(idx_t n)
{
    /* static data is constructed once on first use – works for plain
       integers and for the arbitrary‑precision class because the
       compiler will default‑construct the elements. */
    static seq_t cache[10001];      /* supports up to n = 10000 */
    static idx_t max_computed = 0; /* highest index currently valid */
    static bool init = false;

    /* initialise the first four terms once */
    if (!init)
    {
        cache[1] = (seq_t)64;
        cache[2] = (seq_t)1448;
        cache[3] = (seq_t)32761;
        cache[4] = (seq_t)645627;
        max_computed = 4;
        init = true;
    }

    if (n < 1)          /* offset is 1; values for n<1 are undefined */
        return (seq_t)0;

    if (n <= max_computed)   /* already computed */
        return cache[n];

    /* Extend the cache if requested n is beyond the allocated array.
       The problem statement guarantees that n ≤ 10000, so this
       path should never be used. */
    if (n > 10000)
        abort();          /* silently fail in an unlikely event */

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        seq_t t1 = cache[i - 1];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        cache[i] = (seq_t)19 * t1 + (seq_t)304 * t3 + (seq_t)256 * t4;
    }

    max_computed = n;
    return cache[n];
}
