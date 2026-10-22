/*
 * A398364  a(n) = Sum_{k=0..floor(n/7)} binomial(2*k,k) *
 *          binomial(n-5*k,2*k).
 * https://oeis.org/A398364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/7)} binomial(2*k,k) *
 *            binomial(n-5*k,2*k)
 * Range:     u8 n <= 16, u16 n <= 28, u32 n <= 52, u64 n <= 99, u128 n <= 193,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"   /* provides static seq_t binom(idx_t a, idx_t b) */
#include "../utils/seq_mul.h" /* provides seq_t seq_mul(seq_t a, seq_t b) */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t maxk = n / 7;                     /* floor(n/7) */
    for (idx_t k = 0; k <= maxk; ++k) {
        /* binomial(2*k, k) */
        seq_t t1 = binom(2 * k, k);
        /* binomial(n-5*k, 2*k) */
        seq_t t2 = binom(n - 5 * k, 2 * k);
        /* product */
        seq_t prod = seq_mul(t1, t2);
        /* accumulate */
        sum += prod;
    }
    return sum;
}
