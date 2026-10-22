/*
 * A358243  Number of n-regular, N_0-weighted pseudographs on 2 vertices with
 *          total edge weight 3, up to isomorphism.
 * https://oeis.org/A358243
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (26*n + (-1)^n - 45)/4 for n >= 4; a(1)=1, a(2)=4, a(3)=9
 * Range:     u8 n <= 11, u16 n <= 2556, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)4;
    if (n == 3) return (seq_t)9;

    /* n >= 4 */
    seq_t num = (seq_t)26 * (seq_t)n;
    if (n & 1ull)          /* odd n -> (-1)^n = -1 */
        num -= (seq_t)1;
    else                   /* even n -> (-1)^n =  1 */
        num += (seq_t)1;
    num -= (seq_t)45;
    return num / (seq_t)4;
}
