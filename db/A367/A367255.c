/*
 * A367255  a(n) = (n + 1)^(n - 2)*(3*n + 1).
 * https://oeis.org/A367255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n + 1)^(n - 2) * (3*n + 1) (with integer arithmetic
 *            handling n < 2)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 16, u128 n <= 19,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n + 1;
    seq_t term = 3 * (seq_t)n + 1;

    if (n < 2) {
        idx_t exp = 2 - n;            /* positive exponent (2-n) */
        seq_t denom = big_pow(base, (seq_t)exp);
        return term / denom;
    } else {
        idx_t exp = n - 2;            /* non‑negative exponent (n-2) */
        seq_t pow_val = big_pow(base, (seq_t)exp);
        return pow_val * term;
    }
}
