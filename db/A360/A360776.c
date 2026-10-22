/*
 * A360776  Expansion of Sum_{k>=0} (x * (k + x^3))^k.
 * https://oeis.org/A360776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/4)} (n-3*k)^(n-4*k) * binomial(n-3*k,k)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 4;
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t base_idx = n - 3 * k;
        idx_t exp_idx  = n - 4 * k;

        seq_t base  = (seq_t)base_idx;
        seq_t exp   = (seq_t)exp_idx;

        seq_t power = big_pow(base, exp);
        seq_t b     = binomial(base_idx, k);
        sum += power * b;
    }
    return sum;
}
