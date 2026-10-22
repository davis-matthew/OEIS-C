/*
 * A327296  a(n) is the decimal value of the largest binary palindrome that
 *          can be obtained from the digits of the binary representation of n.
 * https://oeis.org/A327296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = largest binary palindrome obtainable from the digits of n
 * Range:     u8 n <= 256, u16 n <= 4095, u32 n <= 4095, u64 n <= 4095,
 *            u128 n <= 4095, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* a(0) = 0 */

    /* Count ones and zeros in the binary representation of n */
    idx_t c1 = 0;                       /* number of '1' bits */
    idx_t temp = n;
    while (temp)
    {
        if (temp & 1ULL)
            c1++;
        temp >>= 1;
    }

    /* Length of binary representation (without leading zeros) */
    idx_t bitlen = floor_log2_uint64(n) + 1;

    idx_t c0 = bitlen - c1;             /* number of '0' bits */

    /* Basic parameters */
    idx_t pair11   = c1 >> 1;                 /* how many 1-1 pairs can we form */
    idx_t pair0    = c0 >> 1;                 /* how many 0-0 pairs we can form */
    idx_t pairs_max = (c1 + c0) >> 1;         /* maximum possible pairs */

    /* Determine the number of pairs used in the palindrome */
    idx_t p;                                 /* number of pairs in the palindrome */
    if (pair11 == 0)
        p = 0;
    else
    {
        p = pair11 + pair0;
        if (p > pairs_max)
            p = pairs_max;
    }

    /* Number of 1-1 pairs actually used */
    idx_t p1 = (p <= pair11) ? p : pair11;

    /* Remaining digits after using pairs */
    idx_t rem1 = c1 - 2 * p1;
    idx_t rem0 = c0 - 2 * (p - p1);

    /* Middle bit handling */
    bool   middle_present = false;
    seq_t  middle_bit     = 0;
    if (rem1 > 0)
    {
        middle_present = true;
        middle_bit     = (seq_t)1;
    }
    else if (rem0 > 0)
    {
        middle_present = true;
        middle_bit     = (seq_t)0;
        rem0--;         /* consume one zero used in the middle */
    }

    /* Total length of the palindrome */
    idx_t L = 2 * p + (middle_present ? 1 : 0);

    /* Build the palindrome bit by bit (left to right) */
    seq_t result = 0;

    /* Left half */
    for (idx_t i = 0; i < p; ++i)
    {
        seq_t bit = (i < p1) ? (seq_t)1 : (seq_t)0;   /* 1-1 pairs first, then zeros */
        result = (result << 1) | bit;
    }

    /* Middle bit, if present */
    if (middle_present)
        result = (result << 1) | middle_bit;

    /* Right half (mirror of left half) */
    for (idx_t i = 0; i < p; ++i)
    {
        idx_t j = p - 1 - i;                      /* index in the left half to mirror */
        seq_t bit = (j < p1) ? (seq_t)1 : (seq_t)0;
        result = (result << 1) | bit;
    }

    return result;
}
