/*
 * A326920  Constant term in the expansion of (-1 + Product_{k=1..n} (1 + x_k
 *          + 1/x_k))^n.
 * https://oeis.org/A326920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * A002426(k)^n
 *            where A002426(k) is the k-th central trinomial coefficient
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

static seq_t central_trinomial(idx_t k)
{
    seq_t sum = 0;
    idx_t max_i = k / 2;
    for (idx_t i = 0; i <= max_i; ++i) {
        seq_t term = binomial(k, i);
        term = term * binomial(k - i, i);
        sum += term;
    }
    return sum;
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    seq_t total = 0;
    seq_t exp_n = (seq_t)n;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t c = central_trinomial(k);
        seq_t term = big_pow(c, exp_n);
        term = term * binomial(n, k);
        if (((n - k) & 1) != 0) {
            total -= term;
        } else {
            total += term;
        }
    }
    return total;
}
