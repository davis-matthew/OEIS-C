/*
 * A372884  a(n) is the sum of all symmetric peaks in the set of flattened
 *          Catalan words of length n.
 * https://oeis.org/A372884
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (63 + 3^n + 2*(3^n - 45)*n + 18*n^2)/144
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 16, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    if (n < 3u) return 0;
    seq_t pow3n = pow3(n);
    seq_t N = (seq_t)n;
    seq_t sum = (seq_t)63 + pow3n
            + (seq_t)2 * ((pow3n - (seq_t)45) * N)
            + (seq_t)18 * N * N;
    return sum / (seq_t)144;
}
