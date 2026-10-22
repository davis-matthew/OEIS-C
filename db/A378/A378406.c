/*
 * A378406  a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) *
 *          binomial(n+2*k,n-2*k).
 * https://oeis.org/A378406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n,k) * binomial(n+2*k,
 *            n-2*k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 31, u128 n <= 60,
 *            big any n
 * Helpers:   binom, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t limit = n / 2;
    for (idx_t k = 0; k <= limit; ++k) {
        seq_t b1 = binom(n, k);
        seq_t b2 = binom(n + 2 * k, n - 2 * k);
        seq_t term = seq_mul(b1, b2);
        sum = sum + term;
    }
    return sum;
}
