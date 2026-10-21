/*
 * A260404  6th level factorials: product of first n 5th level factorials.
 * https://oeis.org/A260404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{j=1..n} j^C(n-j+5,5) (6th level factorials)
 * Range:     u8 n <= 3, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow, binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/big_pow.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t result = 1;
    for (idx_t j = 1; j <= n; ++j) {
        idx_t m = n - j + 5;
        seq_t exp = binomial(m, 5);
        seq_t pow = big_pow((seq_t)j, exp);
        result = seq_mul(result, pow);
    }
    return result;
}
