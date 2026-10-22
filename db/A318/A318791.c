/*
 * A318791  Prime generating polynomial: a(n) = 9*n^2 - 249*n + 1763.
 * https://oeis.org/A318791
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 249*n + 1763
 * Range:     u8 none, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1‑based; formula is valid for all n >= 1 */
    seq_t n1 = (seq_t)n;            /* promote to seq_t */
    seq_t term = 9 * n1 * n1;       /* 9 * n^2 */
    term -= 249 * n1;               /* subtract 249*n */
    term += 1763;                   /* add constant term */
    return term;
}
