/*
 * A356728  The number of 3-permutations that avoid the patterns 132 and 213.
 * https://oeis.org/A356728
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-1) + n^2 - 3*n (n ≥ 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
/* The utility function pow2 is provided by the build environment. */
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The sequence is defined for n ≥ 1.  For n == 0 we return 0
       to avoid underflow when computing (n-1). */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Compute 2^(n-1) in an arbitrary‑precision type. */
    seq_t two_pow = pow2((idx_t)(n - 1));

    /* 3 * 2^(n-1) */
    seq_t term_pow = (seq_t)3 * two_pow;

    /* n^2 */
    seq_t n_seq = (seq_t)n;
    seq_t term_sq = n_seq * n_seq;

    /* 3*n */
    seq_t term_three_n = (seq_t)3 * n_seq;

    /* a(n) = term_pow + term_sq - term_three_n */
    seq_t result = term_pow + term_sq - term_three_n;
    return result;
}
