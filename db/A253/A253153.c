/*
 * A253153  Number of (n+1) X (2+1) 0..1 arrays with every 2 X 2 subblock
 *          diagonal minus antidiagonal sum nondecreasing horizontally and
 *          vertically.
 * https://oeis.org/A253153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9*2^(n-1) + 48 for n > 3; a(1)=39, a(2)=72, a(3)=88
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Handle the small initial terms explicitly */
    if (n == 1) return (seq_t)39;
    if (n == 2) return (seq_t)72;
    if (n == 3) return (seq_t)88;

    /* For n > 3 use the closed‑form expression */
    seq_t v = pow2(n - 1);   /* 2^(n-1) */
    v = v * (seq_t)9;        /* 9 * 2^(n-1) */
    v = v + (seq_t)48;       /* +48 */
    return v;
}
