/*
 * A338226  a(n) = Sum_{i=0..n-1} i*10^i - Sum_{i=0..n-1} (n-1-i)*10^i.
 * https://oeis.org/A338226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = ((9*n - 11)*10^n + (9*n + 11))/81 Offset: a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 21,
 *            big any n
 * Helpers:   pow10, pow10_128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"
#include "../utils/pow10_128.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 1 */
    if (n <= 1) return (seq_t)0;

    /* For 64‑bit values use 128‑bit intermediates to avoid overflow */
    if (sizeof(seq_t) <= 8) {
        __uint128_t pow_n      = pow10_128(n);
        __uint128_t coeff_minus = (9 * n - 11);
        __uint128_t coeff_plus  = (9 * n + 11);
        __uint128_t numerator = coeff_minus * pow_n + coeff_plus;
        return (seq_t)(numerator / 81);
    }

    /* For arbitrary‑precision seq_t use the provided pow10 routine */
    seq_t pow_n      = pow10(n);
    seq_t coeff_minus = (seq_t)(9 * n - 11);
    seq_t coeff_plus  = (seq_t)(9 * n + 11);
    seq_t numerator = coeff_minus * pow_n + coeff_plus;
    return numerator / (seq_t)81;
}
