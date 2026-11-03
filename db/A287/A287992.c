/*
 * A287992  Number of (undirected) paths in the prism graph Y_n.
 * https://oeis.org/A287992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*2^(n + 1) - 5*n - n^2 - 13)*n
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 24, u64 n <= 54, u128 n <= 117,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 971 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Handle n == 0 (outside the OEIS offset), though not required for the problem */
    if (n == 0ULL) return (seq_t)0;

    /* Convert index to seq_t for arithmetic */
    seq_t n_seq = (seq_t)n;

    /* Compute 2^(n+1) in a type safe way using the provided big_pow helper. */
    idx_t exp = n + 1ULL;
    seq_t pow2 = ipow((seq_t)2, (seq_t)exp);

    /* Compute the numerator: 5*pow2 - 5*n - n^2 - 13 */
    seq_t term = (seq_t)5 * pow2;
    term -= (seq_t)5 * n_seq;
    term -= n_seq * n_seq;
    term -= (seq_t)13;

    /* Multiply by n to get the final value */
    term *= n_seq;
    return term;
}
