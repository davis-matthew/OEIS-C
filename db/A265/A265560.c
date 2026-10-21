/*
 * A265560  Smallest base-3 palindrome m >= n, written in base 10.
 * https://oeis.org/A265560
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest base‑3 palindrome >= n (in base 10) */
#include <stdbool.h>
#include <stdint.h>

#define MAX_DIGITS 70   /* enough for 64‑bit numbers in base 3 */

seq_t a(idx_t n)
{
    /* Special case n == 0 */
    if (n == (idx_t)0) return (seq_t)0;

    /* Convert n to base‑3 digits (most significant first) */
    uint8_t digits[MAX_DIGITS];
    idx_t temp = n;
    int len = 0;
    while (temp > (idx_t)0) {
        digits[len++] = (uint8_t)(temp % (idx_t)3);
        temp /= (idx_t)3;
    }
    /* Reverse to get most significant digit at index 0 */
    for (int i = 0; i < len / 2; ++i) {
        uint8_t t = digits[i];
        digits[i] = digits[len - 1 - i];
        digits[len - 1 - i] = t;
    }

    /* Build the palindrome from the prefix */
    int prefix_len = (len + 1) / 2;
    seq_t pal = (seq_t)0;
    for (int i = 0; i < len; ++i) {
        uint8_t d = (i < prefix_len) ? digits[i] : digits[len - 1 - i];
        pal = pal * (seq_t)3 + (seq_t)d;
    }
    if (pal >= (seq_t)n) return pal;

    /* Increment the prefix (base‑3) */
    int idx = prefix_len - 1;
    bool carry = true;
    while (idx >= 0 && carry) {
        uint8_t new_d = digits[idx] + 1;
        if (new_d == (uint8_t)3) {
            digits[idx] = 0;
            carry = true;
        } else {
            digits[idx] = new_d;
            carry = false;
        }
        --idx;
    }

    /* If there was an overflow, minimal palindrome is of length len+1 */
    if (carry) {
        seq_t pow3 = (seq_t)1;
        for (int i = 0; i < len; ++i) pow3 *= (seq_t)3;
        return pow3 + (seq_t)1;
    }

    /* Mirror updated prefix to right half */
    for (int i = 0; i < prefix_len; ++i) {
        digits[len - 1 - i] = digits[i];
    }

    /* Compute the new palindrome value */
    pal = (seq_t)0;
    for (int i = 0; i < len; ++i) {
        pal = pal * (seq_t)3 + (seq_t)digits[i];
    }

    return pal;
}
