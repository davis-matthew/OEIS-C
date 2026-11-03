/*
 * A271254  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 323", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271254
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 10*2^n + 12 (for n ≥ 2) Initial values: a(0) = 1,
 *            a(1) = 5
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)5;

    /* 4*4^n */
    seq_t pow4n = pow4(n);
    pow4n = pow4n * (seq_t)4;

    /* 10*2^n */
    seq_t pow2n = pow2(n);
    pow2n = pow2n * (seq_t)10;

    return pow4n - pow2n + (seq_t)12;
}
