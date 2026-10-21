/*
 * A273796  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 942", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273796
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A273796:
   a(0) = 1, a(1) = 5
   a(n) = 2*4^n - 16*3^(n-2) + 2^n + 1       for n > 1
*/

#include <stdint.h>

/* Helper: compute base^exp in type seq_t (works for builtin
   integer types as well as arbitrary‑precision classes). */
static inline seq_t pow_seq(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    for (idx_t i = 0; i < exp; ++i)
        result *= base;
    return result;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)5;

    /* term1 = 2 * 4^n */
    seq_t term1 = pow_seq((seq_t)4, n);
    term1 *= (seq_t)2;

    /* term2 = 16 * 3^(n-2)    (n >= 2, so n-2 is non‑negative) */
    seq_t term2 = pow_seq((seq_t)3, n - 2);
    term2 *= (seq_t)16;

    /* term3 = 2^n */
    seq_t term3 = pow_seq((seq_t)2, n);

    /* a(n) = term1 - term2 + term3 + 1 */
    return term1 - term2 + term3 + (seq_t)1;
}
