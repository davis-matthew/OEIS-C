/*
 * A323768  a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)^k.
 * https://oeis.org/A323768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(n-k,k)^k
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 13, u64 n <= 18, u128 n <= 25,
 *            big any n
 * Helpers:   binomial, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    idx_t maxk = n / 2;
    seq_t sum = 0;
    for (idx_t k = 0; k <= maxk; ++k) {
        seq_t bin = binomial(n - k, k);
        seq_t term = pow_seq(bin, (int)k);   // exponent fits in int (k <= n/2 <= 5000)
        sum += term;
    }
    return sum;
}
