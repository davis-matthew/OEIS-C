/*
 * A313744  Coordination sequence Gal.6.345.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A313744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n, except * a(0) = 1, * a(n) = 5*n + 1 for n ≡ 5 (mod
 *            12), * a(n) = 5*n – 1 for n ≡ 7 (mod 12)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t res = (seq_t)5 * (seq_t)n;
    if (n == 0) {
        res += (seq_t)1;
    } else {
        idx_t mod12 = n % 12;
        if (mod12 == 5)
            res += (seq_t)1;
        else if (mod12 == 7)
            res -= (seq_t)1;
    }
    return res;
}
