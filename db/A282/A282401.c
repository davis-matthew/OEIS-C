/*
 * A282401  Eisenstein series E_28(q) (alternate convention E_14(q)),
 *          multiplied by 3392780147.
 * https://oeis.org/A282401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 3392780147; a(n) = 6960 * sigma_27(n) for n >= 1
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 3, u128 n <= 19,
 *            big any n
 * Helpers:   sigma_k
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/sigma_k.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)3392780147;
    }
    seq_t s = sigma_k(n, 27);
    return s * 6960;
}
