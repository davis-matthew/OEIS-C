/*
 * A349116  a(n) = Sum_{m=1..n} (Sum_{k=1..m} (Sum_{j=1..k} j^n)).
 * https://oeis.org/A349116
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{j=1..n} binomial(n-j+2, 2) * j^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t j = 1; j <= n; ++j) {
        idx_t t   = n - j + 2;
        seq_t bin = binomial(t, (idx_t)2);
        seq_t p   = big_pow((seq_t)j, (seq_t)n);
        sum += bin * p;
    }
    return sum;
}
