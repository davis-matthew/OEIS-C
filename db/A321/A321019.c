/*
 * A321019  Coordination sequence for 3-D tiling (Cairo tiling) X Z, with
 *          respect to a 5-valent point.
 * https://oeis.org/A321019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 5, a(2) = 16 For n ≥ 3: if n is even: a(n) =
 *            4*n^2 – 1 if n is odd: let h = (n+1)/2 (computed as
 *            (n>>1)+(n&1)) if h is odd: a(n) = 4*n^2 – 2 else: a(n) = 4*n^2
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* handle the first three terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)16;

    /* compute 4*n*n */
    seq_t N  = (seq_t)n;
    seq_t nn = N * N;
    seq_t val = nn * (seq_t)4;

    if ((n & 1ULL) == 0) {                /* n is even */
        return val - (seq_t)1;
    } else {                              /* n is odd */
        /* (n+1)/2 without overflow: (n>>1)+(n&1) */
        uint64_t h = (n >> 1) + (n & 1);
        if (h & 1ULL) {                   /* h is odd */
            return val - (seq_t)2;
        } else {                          /* h is even */
            return val;
        }
    }
}
