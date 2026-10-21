/*
 * A285895  Sum of divisors d of n such that n/d is not congruent to 0 mod 4.
 * https://oeis.org/A285895
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = sum_{d | n, (n/d) mod 4 != 0} d
 * Range:     u8 n <= 119, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d = i;
            idx_t e = n / i;
            if (e % 4 != 0) sum += (seq_t)d;
            if (d % 4 != 0 && d != e) sum += (seq_t)e;
        }
    }
    return sum;
}
