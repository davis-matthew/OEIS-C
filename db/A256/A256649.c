/*
 * A256649  29-gonal pyramidal numbers: a(n) = n*(n+1)*(9*n-8)/2.
 * https://oeis.org/A256649
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(9*n-8)/2
 * Range:     u8 n <= 3, u16 n <= 24, u32 n <= 984, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t t1  = nn * (nn + (seq_t)1);       /* n*(n+1) */
    seq_t half = t1 / (seq_t)2;             /* (n*(n+1))/2 */
    seq_t term2 = ((seq_t)9 * nn) - (seq_t)8; /* 9*n - 8 */
    return half * term2;                    /* result */
}
