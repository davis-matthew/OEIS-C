/*
 * A270130  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 89", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 4*2^n + 1 for n >= 3; a(0)=1, a(1)=4, a(2)=44
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Utility to compute 2^exp as seq_t, using repeated multiplication */
static seq_t pow2_loop(idx_t exp)
{
    seq_t r = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        r = r * (seq_t)2;
    return r;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)44;

    /* For n >= 3 use the closed‑form */
    idx_t e1 = 2 * n + 2;   /* exponent for 2^(2n+2) = 4*4^n */
    idx_t e2 = n + 2;       /* exponent for 2^(n+2)  = 4*2^n     */

    seq_t term1 = pow2_loop(e1);   /* 2^(2n+2) */
    seq_t term2 = pow2_loop(e2);   /* 2^(n+2)  */

    /* a(n) = 2^(2n+2) - 2^(n+2) + 1 */
    return term1 - term2 + (seq_t)1;
}
