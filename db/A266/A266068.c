/*
 * A266068  Binary representation of the n-th iteration of the "Rule 3"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 10^(n/2)                       for n even
 *          a(n) = (10*100^n - 99*10^((n-1)/2) - 1)/9  for n odd
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Even n: simple power of 10 */
    if ((n & 1) == 0) {
        return pow10(n >> 1);                /* n/2 */
    }

    /* Odd n: use the closed‑form expression */
    idx_t exp_term1 = (n << 1) + 1;          /* 2*n + 1 */
    seq_t term1 = pow10(exp_term1);          /* 10^(2n+1) */

    seq_t term2 = pow10((n - 1) >> 1);       /* 10^((n-1)/2) */
    seq_t term2_mul = term2 * (seq_t)99;     /* 99 * term2 */

    /* numerator: term1 - term2_mul - 1 */
    seq_t numer = term1 - term2_mul - (seq_t)1;

    /* exact division by 9 */
    return numer / (seq_t)9;
}
