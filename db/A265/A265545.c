/*
 * A265545  a(n) = smallest base-3 palindrome m >= n such that every base-3
 *          digit of n is <= the corresponding digit of m; m is written in
 *          base 3.
 * https://oeis.org/A265545
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal integer whose decimal digits are the base‑3 *
 *            digits of the smallest base‑3 palindrome m ≥ n such that for
 *            each * position i, the base‑3 digit of n is ≤ the corresponding
 *            digit of m
 * Range:     u8 n <= 26, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   max8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/max8.h"

seq_t a(idx_t n) {
    /* Special case: n = 0 → 0 */
    if (n == 0) return 0;

    /* Convert n to base‑3, least significant digit first. */
    uint8_t d[64];          /* 64 is plenty for 3^40 > 2^64. */
    int len = 0;
    idx_t tmp = n;
    while (tmp > 0) {
        d[len++] = (uint8_t)(tmp % 3);
        tmp /= 3;
    }

    /* Build the minimal palindrome with the required digitwise
     * constraints.  For indices i < k (k = (len+1)/2), set both
     * mirrored positions to the maximum of the two original digits. */
    uint8_t p[64];
    int k = (len + 1) / 2;
    for (int i = 0; i < k; ++i) {
        uint8_t val = max8(d[i], d[len - 1 - i]);
        p[i] = p[len - 1 - i] = val;
    }

    /* Convert the digit sequence p (most significant first) into a
     * decimal integer.  The digits p are base‑3 digits but we interpret
     * them as decimal digits for the output. */
    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        result = result * 10 + p[i];
    }
    return result;
}
