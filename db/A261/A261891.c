/*
 * A261891  Least k>0 such that n AND (k*n) = 0, where AND stands for the
 *          binary AND operator.
 * https://oeis.org/A261891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = min{k>0 | n & (k*n) == 0}
 * Range:     u8 n <= 254, u16 n <= 16384, u32 n <= 16384, u64 n <= 16384,
 *            u128 n <= 16384, big any n
 * Validated: standard, pass, 93 terms through n = 8374 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to sequence type for operations */
    seq_t nn = (seq_t)n;
    /* start with k = 1, product = n * 1 */
    seq_t prod = nn;
    idx_t   k = 1;

    /* increment k until the AND condition is satisfied */
    for (;; ++k)
    {
        if ((prod & nn) == 0)
            return (seq_t)k;      /* k fits in seq_t for the tests that matter */

        prod += nn;                 /* product = n * (k+1) for next iteration */
    }
}
