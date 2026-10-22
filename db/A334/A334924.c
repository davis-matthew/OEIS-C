/*
 * A334924  G.f.: Sum_{k>=1} x^(k^2*(k + 1)/2) / (1 - x^(k^2*(k + 1)/2)).
 * https://oeis.org/A334924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pentagonal pyramidal numbers dividing n
 *            Pentagonal pyramidal numbers are P(k) = k^2*(k+1)/2 (k ≥ 1)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) returns how many P(k) divide n.  The sequence is OEIS A334924. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t k = 1; ; ++k)
    {
        seq_t kk   = (seq_t)k * (seq_t)k;          /* k^2                     */
        seq_t p    = kk * ((seq_t)k + 1) / 2;     /* k^2*(k+1)/2             */
        if (p > (seq_t)n)
            break;
        if (((seq_t)n % p) == 0)
            ++count;
    }
    return count;
}
