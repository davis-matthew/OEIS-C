/*
 * A346151  a(n) is the smallest integer k > 0 such that 1 - tanh(k) <
 *          10^(-n).
 * https://oeis.org/A346151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil((n*log(10)+log(2))/2)
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Scaling: use 10^18 as common denominator.
       log(10) ≈ 2.302585092994046 * 10^18 = 2302585092994046000  */
    const __int128 LOG10_NUM = (__int128)2302585092994046000LL;
    /* log(2)  ≈ 0.693147180559945309 * 10^18 = 693147180559945309   */
    const __int128 LOG2_NUM  = (__int128)693147180559945309LL;
    /* Denominator 2 * 10^18 */
    const __int128 DEN       = (__int128)2000000000000000000LL;

    /* Numerator: n*log10 + log2, all scaled by 10^18 */
    __int128 num = (__int128)n * LOG10_NUM + LOG2_NUM;
    /* Ceil division: (num + DEN - 1) / DEN */
    __int128 res = (num + DEN - 1) / DEN;

    return (seq_t)res;
}
