/*
 * A271662  Convolution of nonzero pentagonal numbers (A000326) with
 *          themselves.
 * https://oeis.org/A271662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)*(n+2)*(n+3)*(9*n^2 + 21*n + 20)/120
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 140, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_gcd.h"

seq_t a(idx_t n) {
    seq_t n_seq = (seq_t)n;
    seq_t f0 = n_seq + 1;
    seq_t f1 = n_seq + 2;
    seq_t f2 = n_seq + 3;
    seq_t f3 = 9 * n_seq * n_seq + 21 * n_seq + 20;

    seq_t factors[4] = { f0, f1, f2, f3 };
    seq_t div = 120;
    seq_t res = 1;

    for (int i = 0; i < 4; ++i) {
        seq_t g = seq_gcd(factors[i], div);
        seq_t part = factors[i] / g;
        div = div / g;
        res = res * part;
    }
    res = res * div;
    return res;
}
