/*
 * A360925  Smallest number of moves needed to win Integer Lunar Lander from
 *          starting position (n,0).
 * https://oeis.org/A360925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n+1+floor(sqrt(2*n^2-2*n-3)) for n>=2; a(0)=0, a(1)=1
 * Range:     u8 n <= 6, u16 n <= 78, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t nn = (seq_t)n;
    seq_t val = (seq_t)2 * nn * nn - (seq_t)2 * nn - (seq_t)3;
    seq_t s = floor_sqrt(val);
    return nn + (seq_t)1 + s;
}
