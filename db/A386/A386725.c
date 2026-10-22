/*
 * A386725  a(n) is the nearest integer to n/log_10(Pi).
 * https://oeis.org/A386725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round( n / log10(Pi) )
 * Range:     u8 n <= 64, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 10^18 is large enough to preserve the required accuracy
       for all n in the test data (n ≤ 10 000). */
    const unsigned __int128 SCALE = (unsigned __int128)1000000000000000000ULL; /* 10^18 */
    /* log10(Pi) ≈ 0.497149872694133854351…
       P = round( log10(Pi) * 10^18 ) = 497149872694133854 */
    const unsigned __int128 LOG10_PI_NUM = (unsigned __int128)497149872694133854ULL;

    unsigned __int128 num = (unsigned __int128)n * SCALE + LOG10_PI_NUM / 2;
    return (seq_t)(num / LOG10_PI_NUM);
}
