/*
 * A274784  Diagonal of the rational function 1/(1-(wxyz + wxy + wxz + wyz +
 *          xz + yz)).
 * https://oeis.org/A274784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..⌊n/2⌋} binomial(n,2k) * binomial(n+k,k) *
 *            binomial(2k,k)^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute the n-th term of A274784. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t kmax = n / 2;          /* floor(n/2) */

    for (idx_t k = 0; k <= kmax; ++k)
    {
        seq_t t1 = binomial(n, 2 * k);          /* C(n,2k)   */
        seq_t t2 = binomial(n + k, k);          /* C(n+k,k)  */
        seq_t t3 = binomial(2 * k, k);          /* C(2k,k)   */

        /* Multiply the four factors: t1 * t2 * t3 * t3 */
        seq_t term = t1 * t2 * t3 * t3;          /* uses arbitrary‑precision ops when seq_t is big */

        sum += term;
    }
    return sum;
}
