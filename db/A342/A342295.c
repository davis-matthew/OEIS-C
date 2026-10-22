/*
 * A342295  a(n) = Sum_{k = 0..n} binomial(n,k)^12.
 * https://oeis.org/A342295
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n,k)^12
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 7, u128 n <= 12,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t half = n / 2;
    for (idx_t k = 0; k <= half; ++k) {
        seq_t c = binomial(n, k);
        seq_t p = big_pow(c, (seq_t)12);
        sum += p;
        if (k != n - k) {
            sum += p;
        }
    }
    return sum;
}
