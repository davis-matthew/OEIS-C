/*
 * A326339  Number of connected simple graphs with vertices {1..n} and no
 *          crossing or nesting edges.
 * https://oeis.org/A326339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 1 if n=0; 0 if n=1; 1 if n=2; 4·3^(n−3) for n≥3 (OEIS
 *            A326339)
 * Range:     u8 n <= 6, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    seq_t pow3 = seq_pow((seq_t)3, n - 3);
    return pow3 * (seq_t)4;
}
