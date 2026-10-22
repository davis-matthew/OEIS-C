/*
 * A320005  Number of proper divisors of n of the form 6*k + 5.
 * https://oeis.org/A320005
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of proper divisors of n of the form 6*k + 5
 * Range:     u8 n <= 9810, u16 n <= 9810, u32 n <= 9810, u64 n <= 9810,
 *            u128 n <= 9810, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1) return 0;

    seq_t cnt = 0;

    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            if (d < n && d % 6 == 5) ++cnt;
            idx_t q = n / d;
            if (q != d && q < n && q % 6 == 5) ++cnt;
        }
    }

    return cnt;
}
