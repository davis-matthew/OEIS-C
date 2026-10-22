/*
 * A382692  a(n) = floor of alternating sum of k-th roots of n, with k >= 2.
 * https://oeis.org/A382692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Sum_{k=2..∞} (-1)^k * n^(1/k) ) = floor( Σ_{k≥1} (
 *            n^(1/(2k)) – n^(1/(2k+1)) ) )
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>


seq_t a(idx_t n)
{
    /* Handle trivial cases. */
    if (n <= 1)
        return 0;

    /* Work in long double to preserve precision; the final
       result fits into 64 bits for the ranges tested. */
    long double nd   = (long double)n;
    long double sum  = 0.0L;

    /* Accumulate the alternating series until the next
       contribution is negligible.  A 20‑digit tolerance
       is more than sufficient for the required accuracy. */
    for (idx_t k = 1; ; ++k)
    {
        long double e1   = 1.0L / (2.0L * k);
        long double e2   = 1.0L / (2.0L * k + 1.0L);
        long double term = powl(nd, e1) - powl(nd, e2);

        sum += term;

        /* Break when the next term is tiny or we have
           already evaluated a safe upper bound on the
           number of terms needed. */
        if (term < 1e-20L)
            break;
        if (k > 20000)        /* safety guard for pathological inputs */
            break;
    }

    /* Convert the accumulated sum to an integer.
       Adding a tiny epsilon guards against rounding
       down when the true value is extremely close
       to an integer.  The floor is first taken as
       an unsigned long long and then promoted to
       the build's seq_t type. */
    uint64_t value = (uint64_t)floorl(sum + 1e-12L);
    return (seq_t)value;
}
