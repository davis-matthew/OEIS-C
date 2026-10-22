/*
 * A366395  a(n) = Sum_{k=1..n} (-1)^(k-1) * binomial(floor(n/k)+2,3).
 * https://oeis.org/A366395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{k=1..n} binomial(k+1,2) * (⌊n/k⌋ mod 2)
 * Range:     u8 n <= 10, u16 n <= 74, u32 n <= 3056, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k) {
        idx_t f = n / k;           /* floor(n/k) */
        if ((f & 1) == 0)          /* add only if f is odd */
            continue;
        /* binomial(k+1,2) = k*(k+1)/2 */
        seq_t term = (seq_t)k * (seq_t)(k + 1) / 2;
        sum += term;
    }
    return sum;
}
