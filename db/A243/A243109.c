/*
 * A243109  a(n) is the largest number smaller than n and having the same
 *          Hamming weight as n, or n if no such number exist.
 * https://oeis.org/A243109
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) is the largest number smaller than n having the same
 *            Hamming weight as n, or n if no such number exists
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* quick return when n has no lower number with the same popcount
       (this covers n = 0 and n = 2^k-1). */
    if ((n & (n + 1)) == 0)
        return (seq_t)n;

    /* locate the rightmost '10' bit-pair in n */
    idx_t a_pos = 0;        /* position of the upper '1'  (i)   */
    idx_t b_pos = 0;        /* position of the lower '0' (i-1) */

    for (idx_t i = 1; i < 64; ++i) {
        if (((n >> i) & 1ULL) && !(((n >> (i - 1)) & 1ULL))) {
            a_pos = i;
            b_pos = i - 1;
            break;
        }
    }

    /* compute the high part: bits above a_pos are unchanged,
       the bit at a_pos is cleared, and the bit at b_pos is set. */
    idx_t high;
    if (a_pos == 63) {
        /* special case: shifting by 64 is undefined */
        high = 0;
    } else {
        high = n & ~(((idx_t)1 << (a_pos + 1)) - 1);
    }
    high &= ~((idx_t)1 << a_pos);          /* clear the '1' at a_pos   */
    high |=  ((idx_t)1 << b_pos);          /* set the '1' at b_pos     */

    /* count the number of ones below b_pos */
    idx_t low_mask  = ((idx_t)1 << b_pos) - 1;
    idx_t low_bits  = n & low_mask;
    int   countLow  = __builtin_popcountll(low_bits);

    /* construct the new lowest part: put all ones as high as possible
       within the b_pos bits. */
    idx_t new_low = ((idx_t)1 << countLow) - 1;   /* lower countLow bits set to 1 */
    new_low <<= (b_pos - countLow);              /* shift them to the top of the block */

    idx_t result = high | new_low;
    return (seq_t)result;
}
