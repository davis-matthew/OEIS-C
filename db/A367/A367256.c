/*
 * A367256  a(n) = Sum_{k=0..n} binomial(n, k) * binomial(n - 1, k - 1) * n^(n
 *          - k).
 * https://oeis.org/A367256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(n, k) * binomial(n-1, k-1) *
 *            n^(n-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    seq_t t_n = (seq_t)n;
    seq_t pow_n_n = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow_n_n *= t_n;
    }

    seq_t result = (seq_t)0;
    seq_t curr_power = pow_n_n;

    for (idx_t k = 0; k <= n; ++k) {
        seq_t bin_n_k = binomial(n, k);
        seq_t bin_nm1_k1;
        if (k == 0) {
            bin_nm1_k1 = (seq_t)0;
        } else {
            bin_nm1_k1 = binomial(n - 1, k - 1);
        }
        seq_t term = bin_n_k * bin_nm1_k1;
        term *= curr_power;
        result += term;
        if (k < n) {
            curr_power /= t_n;
        }
    }

    return result;
}
