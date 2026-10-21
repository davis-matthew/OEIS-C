/*
 * A265541  Largest base-9 palindrome m <= n, written in base 10.
 * https://oeis.org/A265541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   Largest base-9 palindrome m <= n, returned in base 10
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   left_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/left_value.h"

/* Helper: build palindrome from left half. */
static seq_t build_pal(const unsigned char *left, size_t halfLen, bool odd)
{
    seq_t pal = 0;
    size_t i;
    /* first half */
    for (i = 0; i < halfLen; ++i)
        pal = pal * (seq_t)9 + left[i];
    if (odd) {
        /* mirror excluding middle digit */
        for (i = 0; i < halfLen - 1; ++i)
            pal = pal * (seq_t)9 + left[halfLen - 2 - i];
    } else {
        /* full mirror */
        for (i = 0; i < halfLen; ++i)
            pal = pal * (seq_t)9 + left[halfLen - 1 - i];
    }
    return pal;
}

/* Helper: pow9(k) = 9^k */
static seq_t pow9(size_t k)
{
    seq_t p = 1;
    while (k--) p *= (seq_t)9;
    return p;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0u)
        return (seq_t)0u;

    /* Convert n to base 9 digits (least significant first) */
    unsigned char rev[64];
    size_t rev_len = 0;
    seq_t tmp = (seq_t)n;
    while (tmp > (seq_t)0) {
        rev[rev_len++] = (unsigned char)(tmp % (seq_t)9);
        tmp /= (seq_t)9;
    }

    /* Reverse to most significant first */
    unsigned char digits[64];
    for (size_t i = 0; i < rev_len; ++i)
        digits[i] = rev[rev_len - 1 - i];
    size_t len = rev_len;
    size_t halfLen = (len + 1) / 2;

    /* Left half array */
    unsigned char left[64];
    for (size_t i = 0; i < halfLen; ++i)
        left[i] = digits[i];

    /* Candidate palindrome */
    seq_t cand = build_pal(left, halfLen, (len % 2) == 1);
    if (cand <= (seq_t)n)
        return cand;

    /* Decrement left half */
    seq_t leftVal = left_value(left, halfLen);
    leftVal -= (seq_t)1;

    seq_t minLeft = pow9(halfLen - 1);
    if (leftVal < minLeft) {
        /* All 8's of length len-1 */
        seq_t res = 0;
        for (size_t i = 0; i < len - 1; ++i)
            res = res * (seq_t)9 + (seq_t)8;
        return res;
    }

    /* Reconstruct new left digits from leftVal */
    unsigned char newLeft[64];
    seq_t tv = leftVal;
    for (size_t i = halfLen; i > 0; --i)
        newLeft[i - 1] = (unsigned char)(tv % (seq_t)9), tv /= (seq_t)9;

    return build_pal(newLeft, halfLen, (len % 2) == 1);
}
