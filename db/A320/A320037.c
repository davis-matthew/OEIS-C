/*
 * A320037  Write n in binary, then modify each run of 0's by appending one 1,
 *          and modify each run of 1's by appending one 0. a(n) is the decimal
 *          equivalent of the result.
 * https://oeis.org/A320037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal value obtained by writing n in binary, then * for
 *            each maximal run of identical bits appending one bit of the *
 *            opposite bit. The result is interpreted as a binary number
 * Range:     u8 n <= 17, u16 n <= 297, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_log2_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_log2_uint64.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  Return 0 for n==0 as a
       defensive guard. */
    if (n == 0)
        return 0;

    /* Position of the most significant 1‑bit of n. */
    int msb = floor_log2_uint64(n);

    /* Start with the first bit of n. */
    int currentBit = (n >> msb) & 1;
    seq_t res = (seq_t)currentBit;

    /* Process the remaining bits from msb‑1 down to 0. */
    for (int pos = msb - 1; pos >= 0; --pos) {
        int bit = (n >> pos) & 1;
        if (bit == currentBit) {
            /* Same run – output the bit. */
            res = (res << 1) | (seq_t)bit;
        } else {
            /* Run ends – append the opposite bit before starting
               the new run. */
            res = (res << 1) | (seq_t)(1 - currentBit);
            currentBit = bit;
            /* Output the current bit of the new run. */
            res = (res << 1) | (seq_t)bit;
        }
    }

    /* Append the opposite bit for the final run. */
    res = (res << 1) | (seq_t)(1 - currentBit);

    return res;
}
