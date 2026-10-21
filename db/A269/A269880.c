/*
 * A269880  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 4", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269880
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4^{popcount(n+1)} - 4^{popcount(n)}
 * Range:     i8 n <= 13, i16 n <= 127, i32 n <= 127, i64 n <= 127,
 *            i128 n <= 127, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned popcount_idx(idx_t x)
{
    unsigned c = 0;
    while (x) {
        x &= x - 1;
        ++c;
    }
    return c;
}

static seq_t pow4(unsigned k)
{
    seq_t p = 1;
    while (k) {
        p *= 4;
        --k;
    }
    return p;
}

seq_t a(idx_t n)
{
    unsigned pc_n  = popcount_idx(n);
    unsigned pc_n1 = popcount_idx((idx_t)(n + 1));
    seq_t p_n  = pow4(pc_n);
    seq_t p_n1 = pow4(pc_n1);
    return p_n1 - p_n;
}
