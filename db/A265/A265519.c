/*
 * A265519  a(n) = largest base-7 palindrome m <= n such that every base-7
 *          digit of m is <= the corresponding digit of n; m is written in
 *          base 10.
 * https://oeis.org/A265519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Largest base‑7 palindrome m ≤ n such that each base‑7 digit of m is
 * ≤ the corresponding digit of n; m is returned in base 10. */
#include <stdint.h>

seq_t a(idx_t n) {
    /* Extract base‑7 digits (least significant first). */
    unsigned char d[64];
    unsigned len;
    idx_t tmp = n;
    if (tmp == 0) {
        d[0] = 0;
        len = 1;
    } else {
        len = 0;
        while (tmp != 0) {
            d[len++] = tmp % 7;
            tmp /= 7;
        }
    }

    /* If the least significant digit is 0, any palindrome of length >1
     * would have a leading 0 and would not be a valid base‑7
     * representation, so the only possible palindrome is 0. */
    if (d[0] == 0) {
        return (seq_t)0;
    }

    /* Build the largest palindrome of the same length as n. */
    seq_t result = 0;
    seq_t pow = 1; /* 7^i in seq_t */
    for (unsigned i = 0; i < len; ++i) {
        unsigned j = len - 1 - i;
        unsigned digit = (d[i] < d[j]) ? d[i] : d[j];
        result += (seq_t)digit * pow;
        pow *= (seq_t)7;
    }
    return result;
}
