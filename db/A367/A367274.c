/*
 * A367274  a(n) = binomial(n, k) * (n - k)^k where k = floor(n/2).
 * https://oeis.org/A367274
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n, floor(n/2)) * (n - floor(n/2))^floor(n/2)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 24,
 *            big any n
 * Helpers:   binomial, seq_mul, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    idx_t k = n >> 1;                 /* floor(n/2) */
    seq_t bin = binomial(n, k);       /* C(n, k) */
    seq_t base = (seq_t)(n - k);      /* n - k  */
    seq_t pow = seq_pow(base, k);     /* (n-k)^k */
    return seq_mul(bin, pow);         /* bin * (n-k)^k */
}
