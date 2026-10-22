/*
 * A360479  Expansion of Sum_{k>=0} (x * (1 + (k * x)^2))^k.
 * https://oeis.org/A360479
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..floor(n/3)} (n-2*k)^(2*k) * binomial(n-2*k,k)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 27, u128 n <= 45,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    seq_t sum = 0;
    idx_t maxk = n / 3;          /* floor(n/3) */
    for (idx_t k = 0; k <= maxk; ++k) {
        idx_t m = n - 2 * k;          /* base value for power */
        seq_t base = (seq_t)m;
        seq_t pw   = big_pow(base, (seq_t)(2 * k));  /* (n-2k)^(2k) */
        seq_t bin  = binomial(m, k);                 /* C(m,k) */
        seq_t term = pw * bin;                        /* multiply */
        sum = sum + term;
    }
    return sum;
}
