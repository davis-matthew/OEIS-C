/*
 * A256534  Number of ON cells at n-th stage in simple 2-dimensional cellular
 *          automaton (see Comments lines for definition).
 * https://oeis.org/A256534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = 4*A236305(n-1) for n>=1, a(0)=0. For n>=1, let i =
 *            floor_log2(n)+1 and j = n-2^(i-1). Then a(n) = 2^(2i) + 12*j^2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 9632, u64 n <= 9632,
 *            u128 n <= 9632, big any n
 * Helpers:   mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mul_small.h"

static inline idx_t floor_log2_uint64(idx_t x) {
    /* x > 0 here */
    return 63 - __builtin_clzll(x);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    idx_t i = floor_log2_uint64(n) + 1;          /* i >= 1 */
    idx_t base = 1ULL << (i - 1);                /* 2^(i-1) */
    idx_t j = n - base;                          /* 0 <= j < 2^(i-1) */

    seq_t jj   = (seq_t)j * (seq_t)j;            /* j^2   */
    seq_t term = mul_small(jj, 12);              /* 12*j^2*/
    seq_t p    = pow2(2 * i);                    /* 2^(2i) */

    return p + term;
}
