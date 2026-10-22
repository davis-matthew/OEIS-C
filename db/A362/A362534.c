/*
 * A362534  Numerators of the ratio of the symmetry-constrained bound to the
 *          adiabatic bound on polarization transfer in AXn spin-1/2 systems.
 * https://oeis.org/A362534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator(ceil(n/2) / ((2^n / binomial(n,ceil(n/2)))-1))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, gcd, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent: let k = ceil(n/2), C = binomial(n,k). Then a(n) = (k*C)/gcd(k*C, 2^n - C) */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n<1, return 0 */

    idx_t k = (n + 1) / 2;                 /* ceil(n/2) */
    seq_t C = binomial(n, k);             /* binomial coefficient */
    seq_t num = (seq_t)k * C;              /* k * C   */

    seq_t pow2_n = pow2(n);                /* 2^n     */
    seq_t denom = pow2_n - C;              /* 2^n - C */

    seq_t g = gcd(num, denom);             /* gcd(k*C, 2^n - C) */

    return num / g;                        /* numerator after reduction */
}
