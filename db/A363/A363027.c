/*
 * A363027  Sum of divisors of 5*n-4 of form 5*k+2.
 * https://oeis.org/A363027
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = sum of divisors d of (5*n-4) with d ≡ 2 (mod 5)
 * Range:     u8 n <= 51, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)5 * n - 4;   /* m = 5*n-4 */
    seq_t sum = 0;
    /* iterate over divisors up to sqrt(m) */
    for (idx_t i = 1; (seq_t)i * i <= m; ++i) {
        if (m % i == 0) {
            seq_t d1 = (seq_t)i;
            if (d1 % 5 == 2) sum += d1;
            seq_t d2 = m / d1;   /* m / i */
            if (d2 != d1 && d2 % 5 == 2) sum += d2;
        }
    }
    return sum;
}
