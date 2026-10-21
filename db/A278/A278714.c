/*
 * A278714  Denominators of (n-1)*(n-3)/(6*(2*n-1)), for n >= 1. Denominators
 *          of Dedekind sum s(2, 2*n-1).
 * https://oeis.org/A278714
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 1386, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A278714: Denominators of (n-1)*(n-3)/(6*(2*n-1)) in lowest terms */
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    seq_t N, D, g, res;

    if (n == 1) {
        N = (seq_t)0;
    } else if (n == 2) {
        N = (seq_t)1;
    } else {
        N = ((seq_t)n - 1) * ((seq_t)n - 3);
    }

    D = 6 * (((seq_t)n * 2) - 1);
    g = gcd(N, D);
    res = D / g;
    return res;
}
