/*
 * A275495  a(n) = Sum_{k=2..n} floor(n/k) - 2*floor(n/(2*k)).
 * https://oeis.org/A275495
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=2..n} floor(n/k) - 2*floor(n/(2*k)) (equivalently
 *            a(n) = Sum_{k=2..n} (floor(n/k) mod 2))
 * Range:     u8 n <= 370, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ans = 0;
    /* Loop over k from 2 to n; accumulate the parity of floor(n/k) */
    for (idx_t k = 2; k <= n; ++k) {
        ans += (seq_t)((n / k) & (idx_t)1);
    }
    return ans;
}
