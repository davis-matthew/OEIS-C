/*
 * A270287  Number of active (ON, black) cells at stage 2^n-1 of the
 *          two-dimensional cellular automaton defined by "Rule 145", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A270287
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(0)=1, a(1)=4, a(2)=32, a(n)=47*4^(n-2) - 4*2^n + 1 for n>2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Handle the small cases explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)32;

    /* Compute 4^(n-2) and 2^n using the generic big_pow routine */
    seq_t pow4 = big_pow((seq_t)4, (seq_t)(n - 2));
    seq_t pow2 = big_pow((seq_t)2, (seq_t)n);

    /* Apply the closed‑form formula */
    seq_t result = (seq_t)47 * pow4 - (seq_t)4 * pow2 + (seq_t)1;
    return result;
}
