/*
 * A362673  E.g.f. satisfies A(x) = exp( x * exp(x^2) / A(x) ).
 * https://oeis.org/A362673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = n! * Σ_{k=0}^{⌊n/2⌋} (n-2k)^k * (1-(n-2k))^{(n-2k-1)} /
 *            (k! * (n-2k)!)
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 10, i64 n <= 16, i128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation follows the definition directly, using the
   binomial and big_pow utilities provided in the build.
   The routine works for arbitrary‑precision seq_t and for
   64‑bit signed types.  No memoisation is required, the
   computation is O(n) with small additional constant
   factors. */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

/* Note: idx_t is an unsigned 64‑bit integer, seq_t is the
   build‑dependent signed integer type (possibly arbitrary
   precision).  All operations rely only on integer
   arithmetic and the supplied helpers. */

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    seq_t sum = 0;
    idx_t maxk = n >> 1;          /* floor(n/2) */
    seq_t ratio = 1;              /* equals n!/(k!*(n-2k)!) for k=0 */

    for (idx_t k = 0; k <= maxk; ++k)
    {
        idx_t m = n - 2 * k;     /* m = n-2k */

        /* m^k */
        seq_t m_pow = big_pow((seq_t)m, (seq_t)k);  /* 0^0 treated as 1 */

        /* (1-m)^(m-1)   (base = 1 - m)   exponent = m-1 */
        seq_t base = 1 - (seq_t)m;
        idx_t exp = m - 1;
        seq_t power_term;
        if (exp >= 0)
            power_term = big_pow(base, (seq_t)exp);
        else          /* exp == -1, occurs only when m == 0 */
            power_term = (seq_t)1;   /* because base == 1 in this case */

        /* Term = ratio * m_pow * power_term  */
        seq_t term = ratio;
        term *= m_pow;
        term *= power_term;

        sum += term;

        /* Prepare ratio for next k */
        if (k < maxk)
        {
            idx_t next_k = k + 1;
            /* ratio_{k+1} = ratio_k * (n-2*next_k+2)(n-2*next_k+1) / next_k  */
            seq_t num1 = (seq_t)(n - 2 * next_k + 2); /* = n-2k   */
            seq_t num2 = (seq_t)(n - 2 * next_k + 1); /* = n-2k-1 */
            seq_t denom = (seq_t)next_k;
            ratio = ratio * num1;
            ratio = ratio * num2;
            ratio = ratio / denom;
        }
    }
    return sum;
}
