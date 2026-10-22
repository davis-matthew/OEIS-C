/*
 * A344429  a(n) = Sum_{k=1..n} mu(k) * k^n.
 * https://oeis.org/A344429
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=1..n} mu(k) * k^n
 * Range:     i8 n <= 4, i16 n <= 6, i32 n <= 9, i64 n <= 16, i128 n <= 27,
 *            big any n
 * Helpers:   big_pow, mu_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mu_of.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        int mu = mu_of(k);
        if (mu == 0) continue;
        seq_t pw = big_pow((seq_t)k, (seq_t)n);
        sum += (seq_t)mu * pw;
    }
    return sum;
}
