/*
 * A363759  Smallest number that can be written as a sum of a positive n-th
 *          power and a positive (n+1)-th power in 2 different ways.
 * https://oeis.org/A363759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^{n(n+1)} + 1, except a(2) = 17
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n) {
    if (n == (idx_t)2) {
        return (seq_t)17;
    }
    seq_t exp = (seq_t)n * ((seq_t)n + (seq_t)1);
    seq_t val = big_pow((seq_t)2, exp);
    return val + (seq_t)1;
}
