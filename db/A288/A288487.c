/*
 * A288487  Cuboids that fit in square rings from A288486 obtaining a fifth
 *          power.
 * https://oeis.org/A288487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n + 1) * (n^2 + 1)^2
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 84, u64 n <= 7131,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast index to the result type to ensure correct arithmetic */
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn + (seq_t)1;         /* (n^2 + 1) */
    return (sn + (seq_t)1) * sq * sq;      /* (n + 1) * (n^2 + 1)^2 */
}
