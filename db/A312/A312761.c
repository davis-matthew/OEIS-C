/*
 * A312761  Coordination sequence Gal.5.252.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A312761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n -(n/2+2) if n ≡ 0 (mod 4) /* a(n) = 5*n + 1 if n ≡ 1
 *            (mod 4) /* a(n) = 5*n – 2 if n ≡ 2 (mod 4) /* a(n) = 5*n + 3 if
 *            n ≡ 3 (mod 4) (the first eight terms are handled explicitly)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* base values for the first eight indices */
    static const seq_t base[] = {1, 4, 8, 16, 16, 24, 28, 36};
    if (n < 8) {
        return base[(size_t)n];
    }

    /* 5*n fits in seq_t for all n up to the limits of the platform */
    seq_t val = (seq_t)5 * (seq_t)n;
    unsigned mod4 = (unsigned)(n & 3ULL);   /* n % 4 */

    switch (mod4) {
        case 0: {
            seq_t half = (seq_t)(n >> 1);   /* n/2 */
            return val - (half + (seq_t)2);
        }
        case 1:
            return val + (seq_t)1;
        case 2:
            return val - (seq_t)2;
        case 3:
            return val + (seq_t)3;
    }

    /* unreachable, but satisfies the compiler */
    return val;
}
