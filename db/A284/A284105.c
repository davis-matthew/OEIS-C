/*
 * A284105  a(n) = Sum_{d|n, d == 6 (mod 7)} d.
 * https://oeis.org/A284105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d ≡ 6 (mod 7)} d
 * Range:     u8 n <= 257, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
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
            if (i % 7 == 6)
                sum += (seq_t)i;
            idx_t j = n / i;
            if (j != i && j % 7 == 6)
                sum += (seq_t)j;
        }
    }
    return sum;
}
