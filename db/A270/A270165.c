/*
 * A270165  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 107", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270165
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 14*4^(n-1) - 3*2^n + 1 for n > 3 with the initial values
 *            a(0) = 1, a(1) = 5, a(2) = 41, a(3) = 197 This implements the
 *            conjectured closed form for all n ≥ 0
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)41;
    if (n == 3) return (seq_t)197;

    /* For n > 3 use the closed‑form expression */
    seq_t term1 = pow_uint(4, n - 1);      /* 4^(n-1)   */
    term1 *= (seq_t)14;                   /* 14*4^(n-1) */

    seq_t term2 = pow_uint(2, n);          /* 2^n       */
    term2 *= (seq_t)3;                    /* 3*2^n     */

    return term1 - term2 + (seq_t)1;       /* 14*4^(n-1) – 3*2^n + 1 */
}
