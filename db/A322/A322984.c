/*
 * A322984  Number of iterations of A011371(x) = x - A000120(x) needed to
 *          reach an odd number, when starting from x = 2n.
 * https://oeis.org/A322984
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of iterations of f(x)=x−A000120(x) needed to reach
 *            an odd number when starting from x = 2n
 * Range:     u8 n <= 752, u16 n <= 9453, u32 n <= 9453, u64 n <= 9453,
 *            u128 n <= 9453, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline seq_t popcount_seq_t(seq_t x) {
    seq_t cnt = 0;
    while (x) {
        cnt += (x & 1);
        x >>= 1;
    }
    return cnt;
}

seq_t a(idx_t n) {
    /* start with x = 2*n */
    seq_t x = (seq_t)n * 2;
    seq_t steps = 0;
    while ((x & 1) == 0) {         /* while x is even */
        steps++;
        seq_t pc = popcount_seq_t(x);
        x -= pc;
    }
    return steps;
}
