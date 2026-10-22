/*
 * A373189  Expansion of Sum_{k>=0} x^(4^k) / (1 - x^(4^k))^3.
 * https://oeis.org/A373189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = triangular(n) if n % 4 != 0; a(n) = triangular(n) +
 *            a(n/4) if n % 4 == 0. Here triangular(n) = n*(n+1)/2
 * Range:     u8 n <= 22, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* safety guard; OEIS offset starts at 1 */
    seq_t res = 0;
    seq_t nn = (seq_t)n;
    res = nn * (nn + 1) / 2;              /* triangular part for current n */

    while (!(n & 3)) {                    /* while n is divisible by 4 */
        n >>= 2;                          /* n = n / 4 */
        nn = (seq_t)n;
        res += nn * (nn + 1) / 2;          /* add triangular part for n */
    }
    return res;
}
