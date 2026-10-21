/*
 * A295822  Number of coprime pairs (a,b) with -n <= a <= n, -3 <= b <= 3.
 * https://oeis.org/A295822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 if n = 0, * = 4 + 10*n + 2*(n mod 2) - 4*floor(n/3) for
 *            n >= 1
 * Range:     u8 n <= 28, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    /* use conversions to seq_t carefully to avoid overflow of intermediates */
    seq_t N = (seq_t)n;
    seq_t result = 4 + 10 * N;
    result += 2 * (seq_t)(n & 1);       /* add 2 if n is odd */
    result -= 4 * (seq_t)(n / 3);       /* subtract 4*floor(n/3) */
    return result;
}
