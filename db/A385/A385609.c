/*
 * A385609  Partial sums of A090740.
 * https://oeis.org/A385609
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + floor(n/2) - A000120(n)
 * Range:     u8 n <= 103, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn   = (seq_t)n;          /* convert index to seq_t */
    seq_t pop  = 0;                 /* popcount of n */

    /* compute population count without relying on compiler builtins */
    idx_t t = n;
    while (t) {
        pop++;
        t &= t - 1;                 /* clears the least‑significant set bit */
    }

    seq_t term = 2 * sn + (sn >> 1) - pop;
    return term;
}
