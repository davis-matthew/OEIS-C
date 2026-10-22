/*
 * A400869  The number of ordered pairs (k_1, k_2) of nonnegative integers
 *          bitwise covered by n such that k_1 + k_2 is also bitwise covered
 *          by n.
 * https://oeis.org/A400869
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, new
 * Formula:   a(n) = product over runs of 1's in binary representation of n of
 *            2^(k-1)*(2^k+1)
 * Range:     u8 n <= 30, u16 n <= 510, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    idx_t t = n;
    seq_t result = 1;
    idx_t runlen = 0;
    while (t) {
        if (t & 1) {
            runlen++;
        } else {
            if (runlen) {
                seq_t part = pow2(runlen - 1);
                part = part * (pow2(runlen) + 1);
                result = result * part;
                runlen = 0;
            }
        }
        t >>= 1;
    }
    if (runlen) {
        seq_t part = pow2(runlen - 1);
        part = part * (pow2(runlen) + 1);
        result = result * part;
    }
    return result;
}
