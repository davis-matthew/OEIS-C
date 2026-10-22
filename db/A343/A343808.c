/*
 * A343808  Partial sums of A062074.
 * https://oeis.org/A343808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} 3^k * k^3 This implements the summation
 *            directly and works for any seq_t, including big‑integer builds
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 30, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;
    seq_t power  = 1;        /* 3^k, starts with 3^0 = 1 */
    for (idx_t k = 0; k <= n; ++k)
    {
        seq_t kseq = (seq_t)k;
        seq_t kcube = kseq * kseq * kseq;   /* k^3 */
        result += power * kcube;            /* add 3^k * k^3 */
        power *= (seq_t)3;                  /* prepare 3^(k+1) */
    }
    return result;
}
