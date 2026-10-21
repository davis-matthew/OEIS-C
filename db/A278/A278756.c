/*
 * A278756  Decimal representation of the x-axis, from the origin to the right
 *          edge, of the n-th stage of growth of the two-dimensional cellular
 *          automaton defined by "Rule 65", based on the 5-celled von Neumann
 *          neighborhood.
 * https://oeis.org/A278756
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2k) = 2^(2k+1) - 2^k, a(2k+1) = 2^k - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute 2^e as seq_t.  e is small enough for the requested tests
   (at most ~10 000), so a simple loop is adequate and safe for
   all unsigned integer types and for arbitrary‑precision classes that
   overload the multiplication operator. */
static seq_t pow2(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i)
        r *= 2;
    return r;
}

seq_t a(idx_t n)
{
    if ((n & 1) == 0)       /* even n = 2k */
    {
        seq_t high = pow2(n + 1);   /* 2^(n+1) = 2^(2k+1) */
        seq_t low  = pow2(n / 2);   /* 2^(n/2)   = 2^k     */
        return high - low;
    }
    else                    /* odd n = 2k+1 */
    {
        seq_t kpow = pow2((n - 1) / 2);  /* 2^k          */
        return kpow - 1;                 /* 2^k - 1      */
    }
}
