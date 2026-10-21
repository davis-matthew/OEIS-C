/*
 * A248658  G.f.: Sum_{n>=0} x^n * Sum_{k=0..n} binomial(n,k)^3 * x^(2*k).
 * https://oeis.org/A248658
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} C(n-2*k, k)^3
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 42, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Compute the nth term of OEIS A248658 */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */

    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t nn = n - 2 * k;    /* n - 2k  */
        seq_t bin = binomial(nn, k);
        seq_t cube = bin * bin * bin;
        sum += cube;
    }

    return sum;
}
