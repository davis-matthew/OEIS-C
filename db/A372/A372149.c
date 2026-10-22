/*
 * A372149  Palindrome numbers consisting only of odd digits.
 * https://oeis.org/A372149
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = nth palindrome consisting only of odd digits (1,3,5,7,9)
 * Range:     u8 n <= 15, u16 n <= 135, u32 n <= 5935, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static const unsigned char odd_map[5] = {1, 3, 5, 7, 9};

seq_t a(idx_t n)
{
    /* find the length of the nth palindrome */
    idx_t len = 1;               /* current palindrome length   */
    idx_t count = 0;             /* count of palindromes with length < len */
    idx_t powHalf = 5;           /* 5^ceil(len/2), number of palindromes of length len */

    while (1) {
        if (count + powHalf >= n)
            break;               /* the desired palindrome has length len */
        count += powHalf;
        ++len;
        if ((len & 1) == 0) {    /* only change powHalf on odd lengths > 1 */
            /* len is even after increment */
        } else {
            powHalf *= 5;        /* len is odd now, ceil(len/2) increased by 1 */
        }
    }

    idx_t halfLen = (len + 1) >> 1;   /* ceil(len/2) */
    idx_t rem = n - count - 1;        /* zero‑based index within the current block */

    /* Decode rem into base‑5 digits (least significant first) */
    unsigned char indices[64];        /* halfLen ≤ 64 for n up to 10⁶ (this is safe) */
    for (idx_t i = halfLen; i > 0; --i) {
        indices[i - 1] = (unsigned char)(rem % 5);
        rem /= 5;
    }

    /* Build the full list of decimal digits of the palindrome */
    unsigned char all_digits[128];    /* should be more than enough */
    size_t idx = 0;

    /* first half */
    for (idx_t i = 0; i < halfLen; ++i) {
        all_digits[idx++] = odd_map[indices[i]];
    }

    /* second half (mirror of the first half) */
    int start;
    if ((len & 1) == 0)          /* even length: mirror all first half digits */
        start = halfLen - 1;
    else                         /* odd length: omit the middle digit */
        start = (int)halfLen - 2;

    for (int i = start; i >= 0; --i) {
        all_digits[idx++] = odd_map[indices[i]];
    }

    /* Convert the digit array to a seq_t value */
    seq_t value = 0;
    for (size_t i = 0; i < idx; ++i) {
        value = value * 10 + all_digits[i];
    }

    return value;
}
