/*
 * A271005  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 245", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A271005
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*4^n - 6*2^n (n > 2) Initial values: a(0) = 1, a(1) = 8,
 *            a(2) = 44
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)1;
    if (n == 1u) return (seq_t)8;
    if (n == 2u) return (seq_t)44;

    /* Compute 4^n and 2^n using the provided big integer power routine. */
    seq_t pow4n = big_pow((seq_t)4, (seq_t)n);  /* 4^n */
    seq_t pow2n = big_pow((seq_t)2, (seq_t)n);  /* 2^n */

    /* Apply the closed‑form: 4*4^n - 6*2^n */
    seq_t term1 = pow4n * (seq_t)4;          /* 4*4^n */
    seq_t term2 = pow2n * (seq_t)6;          /* 6*2^n */

    return term1 - term2;
}
