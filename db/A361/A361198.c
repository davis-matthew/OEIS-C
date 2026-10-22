/*
 * A361198  Consider a perfect infinite binary tree with nodes labeled with
 *          distinct positive integers where n appears at level A082850(n) and
 *          each level is filled from left to right; a(n) is the sibling of n
 *          in this tree.
 * https://oeis.org/A361198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is obtained from the skew‑binary expansion of n by *
 *            replacing the least significant non‑zero digit d by 3 – d. *
 *            This satisfies the OEIS definition of A361198
 * Range:     u8 n <= 254, u16 n <= 8190, u32 n <= 8190, u64 n <= 8190,
 *            u128 n <= 8190, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* n is 1‑based; 0 is not used but handled defensively. */
    if (n == 0) return (seq_t)0;

    /* Skew‑binary weights  = 2^(i+1) – 1,  i = 0..64  */
    enum { MAX_DIGITS = 65 };
    idx_t  weight[MAX_DIGITS];
    unsigned char digit[MAX_DIGITS] = {0};

    /* Build weight table up to the largest weight <= n */
    weight[0] = (idx_t)1;   /* 2^(0+1) – 1 = 1 */
    idx_t last = 0;         /* index of the largest weight <= n */

    while (1) {
        if (weight[last] > n) break;   /* safety, should not happen */
        if (last == 63) break;         /* next weight would overflow uint64_t */
        weight[last + 1] = (weight[last] << 1) | (idx_t)1; /* w[i+1] = 2*w[i] + 1 */
        ++last;
    }

    /* Decompose n into skew‑binary digits (from most significant to least). */
    idx_t v = n;
    for (idx_t i = last; ; --i) {
        /* Each digit is 0,1 or 2. */
        unsigned int di = (unsigned int)(v / weight[i]);
        if (di > 2) di = 2;            /* for safety, though it never occurs */
        digit[i] = (unsigned char)di;
        v -= (idx_t)di * weight[i];
        if (i == 0) break;
    }

    /* Replace the least significant non‑zero digit by 3 – d. */
    for (idx_t i = 0; i <= last; ++i) {
        if (digit[i] != 0) {
            digit[i] = (unsigned char)(3U - digit[i]);  /* 1 ↔ 2, 2 ↔ 1 */
            break;
        }
    }

    /* Reconstruct the integer from the modified digits. */
    seq_t result = (seq_t)0;
    for (idx_t i = 0; i <= last; ++i) {
        result += (seq_t)(digit[i] * weight[i]);  /* cast to seq_t for final value */
    }

    return result;
}
