/*
 * A352669  Maximum number of induced cycles in an n-node graph.
 * https://oeis.org/A352669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, hard, more
 * Formula:   a(n) = max( binomial(n,3), binomial(floor(n/2),2) *
 *            binomial(ceil(n/2),2) )
 * Range:     u8 n <= 12, u16 n <= 12, u32 n <= 12, u64 n <= 12, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t nseq = (seq_t)n;
    seq_t t1 = nseq * (nseq - 1) * (nseq - 2) / 6;

    idx_t k  = n >> 1;          /* floor(n/2)   */
    idx_t k2 = n - k;           /* ceil(n/2)    */

    seq_t k_choose2  = (seq_t)k  * (k  - 1) / 2;
    seq_t k2_choose2 = (seq_t)k2 * (k2 - 1) / 2;

    seq_t t2 = k_choose2 * k2_choose2;

    return (t1 > t2) ? t1 : t2;
}
