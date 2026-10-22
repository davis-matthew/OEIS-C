/*
 * A355729  Tournament standing, under standard rules double elimination, of
 *          the participant whose elimination leaves n participants still in
 *          the tournament.
 * https://oeis.org/A355729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1 a(1) = 2 For n ≥ 2 let k = ⌊log₂(n)⌋. Let high = 2ᵏ,
 *            half = 2ᵏ⁻¹, offset = n – high. If offset < half → a(n) = high +
 *            1 (first block) Else → a(n) = 3·half + 1 (second block)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* floor_log2 for unsigned integer n ≥ 1 */
static inline int floor_log2_idx(idx_t n)
{
    int k = 0;
    for (idx_t t = n >> 1; t; t >>= 1)
        ++k;
    return k;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)2;

    int k = floor_log2_idx(n);          /* highest set bit position of n   */
    seq_t high = (seq_t)1 << k;          /* 2ᵏ                               */
    seq_t half = (seq_t)1 << (k - 1);    /* 2ᵏ⁻¹                             */
    idx_t offset = n - ((idx_t)1 << k);   /* n – 2ᵏ                           */

    if (offset < half)
        return high + (seq_t)1;        /* first block: 2ᵏ + 1              */
    else
        return (seq_t)3 * half + (seq_t)1; /* second block: 3·2ᵏ⁻¹ + 1   */
}
