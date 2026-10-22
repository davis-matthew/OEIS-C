/*
 * A362300  a(n) = n! * Sum_{k=0..floor(n/3)} (n/3)^k *
 *          binomial(n-2*k,k)/(n-2*k)!.
 * https://oeis.org/A362300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n! * Sum_{k=0..⌊n/3⌋} (n/3)^k * binomial(n-2k,k)/(n-2k)!.
 *
 * Implementation:
 *   T_0 = 1
 *   T_k = T_{k-1} * n * (n-3k+1) * (n-3k+2) * (n-3k+3) / (3*k)
 *   a(n) = Σ_{k=0..⌊n/3⌋} T_k
 *
 * gcd cancellation is used to avoid intermediate overflow on fixed-size
 * integer builds.
 */

#include <stddef.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;        // a(n)
    seq_t term = 1;       // T_0
    sum += term;

    idx_t maxk = n / 3;
    for (idx_t k = 1; k <= maxk; ++k)
    {
        /* Compute T_k from T_{k-1} with gcd cancellation. */
        seq_t temp = term;                          /* current term */
        seq_t den  = (seq_t)3 * k;                  /* denominator 3*k */

        /* Cancel common factors between numerator and denominator. */
        seq_t g = gcd(temp, den);
        temp /= g;
        den  /= g;

        /* Multiplicative factors: n, (n-3k+1), (n-3k+2), (n-3k+3). */
        seq_t fac[4];
        fac[0] = (seq_t)n;
        fac[1] = (seq_t)(n - 3 * k + 1);
        fac[2] = (seq_t)(n - 3 * k + 2);
        fac[3] = (seq_t)(n - 3 * k + 3);

        for (int i = 0; i < 4; ++i)
        {
            g = gcd(fac[i], den);
            fac[i] /= g;
            den    /= g;
            temp   *= fac[i];
        }
        /* After cancellation den should be 1. */
        term = temp;
        sum += term;
    }

    return sum;
}
