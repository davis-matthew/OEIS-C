/*
 * A274902  Number of (not necessarily proper) edge colorings of the truncated
 *          cube using at most n colors.
 * https://oeis.org/A274902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^36 + 6*n^21 + 3*n^20 + 6*n^19 + 4*n^18 + 8*n^12 +
 *            12*n^9 + 8*n^6)/48
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    // Convert index to seq_t
    seq_t nn = (seq_t)n;
    // Compute powers of n up to n^36 iteratively
    seq_t power[37];  // power[i] = n^i
    power[1] = nn;
    for (int i = 2; i <= 36; ++i) {
        power[i] = power[i - 1] * nn;
    }

    // Group required powers
    seq_t n6  = power[6];
    seq_t n9  = power[9];
    seq_t n12 = power[12];
    seq_t n18 = power[18];
    seq_t n19 = power[19];
    seq_t n20 = power[20];
    seq_t n21 = power[21];
    seq_t n36 = power[36];

    // Compute 48 * a(n) = n^36 + 6*n^21 + 3*n^20 + 6*n^19 + 4*n^18 + 8*n^12 + 12*n^9 + 8*n^6
    seq_t num = n36;
    num += n21 * (seq_t)6;
    num += n20 * (seq_t)3;
    num += n19 * (seq_t)6;
    num += n18 * (seq_t)4;
    num += n12 * (seq_t)8;
    num += n9  * (seq_t)12;
    num += n6  * (seq_t)8;

    // Divide by 48 to obtain a(n)
    return num / (seq_t)48;
}
