/*
 * A309000  Number of strings of length n from a 3-symbol alphabet (A,B,C,
 *          say) containing at least one "A" and at least two "B"s.
 * https://oeis.org/A309000
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n - 2^(n+1) - n*2^(n-1) + n + 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Offset: 3  (a(3) is first term) */
#include "../utils/pow_seq.h"

seq_t a(idx_t n)
{
    /* For n < 3 the sequence is not defined; return 0.  */
    if (n < 3)
        return 0;

    /* Compute powers of 2 and 3.  Large exponents fit in int for the
       intended usage range (n up to about 10 000).  */
    seq_t pow3          = pow_seq(3, (int)n);
    seq_t pow2_nplus1   = pow_seq(2, (int)(n + 1));
    seq_t pow2_nminus1  = pow_seq(2, (int)(n - 1));

    /* Apply the closed‑form formula. */
    seq_t result =
        pow3
        - pow2_nplus1
        - ((seq_t)n * pow2_nminus1)
        + (seq_t)n
        + 1;

    return result;
}
