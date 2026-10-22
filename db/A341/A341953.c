/*
 * A341953  Replace each digit d in the decimal representation of n with the
 *          digital root of d^n.
 * https://oeis.org/A341953
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = Replace each decimal digit d of n by the digital * root
 *            of dⁿ and concatenate the resulting digits
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   dr_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>    /* for uint64_t, if needed */
#include "../utils/dr_pow.h"

/* Main sequence function.  idx_t n >= 1.  The result is constructed by
 * processing the decimal digits of n from most significant to least
 * significant, replacing each digit by the digital root of digitⁿ,
 * and concatenating the single‑digit results. */
seq_t a(idx_t n)
{
    /* n is at least 1; for n == 0 we simply return 0 (not used). */
    if (n == 0) return 0;

    /* Collect decimal digits of n in reverse order. */
    int digits[21];      /* uint64_t can have at most 20 decimal digits */
    int len = 0;
    idx_t tmp = n;
    while (tmp) {
        digits[len++] = tmp % 10;
        tmp /= 10;
    }

    seq_t result = 0;
    for (int i = len - 1; i >= 0; --i) {
        int d  = digits[i];
        seq_t dr = dr_pow(n, d);
        result = result * 10 + dr;
    }
    return result;
}
