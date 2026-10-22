/*
 * A361684  Ramsey core number rc(n,n).
 * https://oeis.org/A361684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ceil(2*n - 3/2 + sqrt(2*(n-1)^2 + 9/4))
 * Range:     u8 n <= 6, u16 n <= 1622, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

static seq_t floor_sqrt_seq(seq_t x) {
    if (x < 2) return x;
    unsigned bitlen = bitlen_seq_t(x);
    seq_t low = 0;
    seq_t high = ((seq_t)1) << ((bitlen + 1) / 2);
    while (low < high) {
        seq_t mid = (low + high + 1) >> 1;
        seq_t sq = mid * mid;
        if (sq <= x) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return low;
}

seq_t a(idx_t n) {
    /* n >= 1 by definition of the sequence */
    seq_t sn = (seq_t)n;
    seq_t n_minus1 = sn - 1;
    seq_t nmm = n_minus1 * n_minus1;          /* (n-1)^2   */
    seq_t D = nmm * 8 + 9;                    /* D = 8*(n-1)^2 + 9 */

    seq_t sqrt_D = floor_sqrt_seq(D);
    bool perfect = (sqrt_D * sqrt_D == D);

    seq_t A = sn * 4 - 3;                     /* A = 4*n - 3 */

    if (perfect) {
        seq_t numerator = A + sqrt_D;
        return numerator / 2 + (numerator % 2);
    } else {
        seq_t numerator = A + sqrt_D;
        return numerator / 2 + 1;
    }
}
