/*
 * A319021  Next larger integer with same sum of digits in base 3 as n.
 * https://oeis.org/A319021
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = next larger integer with same sum of base‑3 digits
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* seq_t and idx_t are defined elsewhere by the build. */

seq_t a(idx_t n)
{
    /* Handle trivial case */
    if (n == 0)
        return 0;

    /* Convert n to digits in base 3 */
    seq_t tmp = (seq_t)n;
    unsigned char digits[200];
    size_t len = 0;
    while (tmp > 0) {
        digits[len++] = (unsigned char)(tmp % 3);
        tmp /= 3;
    }
    if (len == 0) {
        digits[0] = 0;
        len = 1;
    }

    /* Sum of digits */
    unsigned int sum = 0;
    for (size_t i = 0; i < len; ++i)
        sum += digits[i];

    /* Find the rightmost position that can be increased by 1
       while there is a lower digit that can be reduced by 1. */
    unsigned int prefix = 0;   /* sum of digits below current position */
    for (size_t i = 0; i < len; ++i) {
        if (digits[i] < 2 && prefix >= 1) {
            /* Build result digits */
            unsigned char res[202];
            size_t res_len = len;          /* same length as original */
            unsigned int needed = prefix - 1;   /* amount to redistribute */

            /* Lower positions (0 .. i-1) : distribute 'needed' as little as possible */
            for (size_t j = 0; j < i; ++j) {
                unsigned int val = (needed > 2) ? 2 : needed;
                res[j] = (unsigned char)val;
                needed -= val;
            }
            /* Remaining lower positions (i .. res_len-1) are unchanged except at i */
            for (size_t j = i; j < res_len; ++j) {
                if (j == i)
                    res[j] = digits[j] + 1;   /* increased digit */
                else
                    res[j] = digits[j];       /* unchanged higher digits */
            }

            /* Assemble result */
            seq_t result = 0;
            for (size_t j = res_len; j > 0; --j)
                result = result * 3 + res[j - 1];
            return result;
        }
        prefix += digits[i];
    }

    /* No such position found – we must extend the length by one digit. */
    size_t res_len = len + 1;
    unsigned char res[202];
    unsigned int needed = sum - 1;   /* remaining sum after the leading 1 */

    /* Lower positions (0 .. res_len-2) : distribute 'needed' */
    for (size_t j = 0; j < res_len - 1; ++j) {
        unsigned int val = (needed > 2) ? 2 : needed;
        res[j] = (unsigned char)val;
        needed -= val;
    }
    res[res_len - 1] = 1;   /* leading digit */

    /* Assemble result */
    seq_t result = 0;
    for (size_t j = res_len; j > 0; --j)
        result = result * 3 + res[j - 1];
    return result;
}
