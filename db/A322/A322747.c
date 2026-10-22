/*
 * A322747  a(n) = sqrt(1 + A322746(2*n)).
 * https://oeis.org/A322747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*n, 2*k) * (2*n+1)^(n-k) * (2*n)^k
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation via explicit sum using provided binomial and big_pow helpers */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    idx_t N = (idx_t)2 * n;          /* 2*n */
    seq_t N_seq = (seq_t)N;
    idx_t M = N + 1;                 /* 2*n+1 */
    seq_t M_seq = (seq_t)M;
    seq_t sum = 0;

    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t bin = binomial(N, 2 * k);
        seq_t pow1 = big_pow(M_seq, (seq_t)(n - k));
        seq_t pow2 = big_pow(N_seq, (seq_t)k);
        sum += bin * pow1 * pow2;
    }

    return sum;
}
