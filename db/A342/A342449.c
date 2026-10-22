/*
 * A342449  a(n) = Sum_{k=1..n} gcd(k,n)^k.
 * https://oeis.org/A342449
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} gcd(k,n)^k
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* n is assumed ≥ 1; behaviour for n == 0 is undefined but handled safely */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t g = gcd((seq_t)k, (seq_t)n);
        seq_t term = big_pow(g, (seq_t)k);
        sum += term;
    }
    return sum;
}
