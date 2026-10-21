/*
 * A299025  a(n) = the fractional part of 1 / A003592(n) read backwards.
 * https://oeis.org/A299025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 29, u64 n <= 101, u128 n <= 356,
 *            big any n
 * Helpers:   nth_hamming, pow5_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A299025
 *  a(n) = the fractional part of 1 / A003592(n) read backwards.
 *  A003592(n) is the n‑th integer of the form 2^i 5^j (i,j ≥ 0).
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/nth_hamming.h"
#include "../utils/pow5_u64.h"

/* helper: integer power of 2 */
static seq_t pow2_u64(idx_t e) {
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 2;
    return r;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, but return 0 */

    seq_t m = nth_hamming(n);
    if (m == 1) return 0;          /* 1/1 = 0.0 → fractional part 0 */

    /* factorise m into 2^exp2 * 5^exp5 */
    idx_t exp2 = 0, exp5 = 0;
    seq_t tmp  = m;

    while ((tmp % 2) == 0) {
        tmp /= 2;
        ++exp2;
    }
    while ((tmp % 5) == 0) {
        tmp /= 5;
        ++exp5;
    }

    /* maximum exponent gives the number of decimal digits */
    idx_t k = exp2 > exp5 ? exp2 : exp5;

    /* numerator of 1/m expressed over 10^k : 5^(k-exp5) * 2^(k-exp2) */
    seq_t numerator = pow5_u64(k - exp5) * pow2_u64(k - exp2);

    /* reverse the k decimal digits of numerator (pad with zeros if needed) */
    seq_t rev = 0;
    seq_t digit;
    seq_t num = numerator;
    for (idx_t i = 0; i < k; ++i) {
        digit = num % 10;
        rev   = rev * 10 + digit;
        num   /= 10;
    }

    return rev;
}
