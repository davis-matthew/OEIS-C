/*
 * A387922  a(n) = 3*binomial(2*n+1,n+1) - n^2 - 2*n - 4.
 * https://oeis.org/A387922
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*binomial(2*n+1,n+1) - n^2 - 2*n - 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* The sequence is defined only for n ≥ 1; return 0 for safety. */
        return 0;
    }

    /* Compute the binomial coefficient C(2*n+1, n+1). */
    idx_t twonplus1 = n * 2 + 1;   /* (2*n+1) as idx_t */
    idx_t np1 = n + 1;             /* (n+1) as idx_t */
    seq_t bin_val = binomial(twonplus1, np1);

    /* Convert n to seq_t for further arithmetic. */
    seq_t n_seq = (seq_t)n;

    /* Compute the formula. */
    seq_t term = (seq_t)3 * bin_val;
    seq_t sub  = n_seq * n_seq + (seq_t)2 * n_seq + (seq_t)4;
    return term - sub;
}
