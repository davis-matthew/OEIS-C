/*
 * A269698  First differences of number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 6", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 253, i32 n <= 299, i64 n <= 299,
 *            i128 n <= 299, big any n
 * Validated: standard, pass, 70 terms through n = 289 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A269698
    a(n) = c(n+1) - c(n)
    where c(n) = 4^{popcount(n)}              if n is even
          c(n) = 5 * 4^{popcount(n)-1}        if n is odd
    (popcount is the number of 1-bits in n)
*/

#include <stdint.h>

/* compute 4^e as seq_t (e <= 64); uses simple multiplication to support
   arbitrary-precision seq_t */
static seq_t pow4_u64(idx_t e)
{
    seq_t r = 1;
    for (idx_t i = 0; i < e; ++i) r *= 4;
    return r;
}

/* number of ON cells after stage n */
static seq_t cell_count(idx_t n)
{
    idx_t pc = __builtin_popcountll(n);      /* popcount of n */
    if (n & 1)                               /* odd n */
        return 5 * pow4_u64(pc - 1);          /* pc>=1 for odd n */
    else                                      /* even n */
        return pow4_u64(pc);
}

/* main sequence function */
seq_t a(idx_t n)
{
    return cell_count(n + 1) - cell_count(n);
}
