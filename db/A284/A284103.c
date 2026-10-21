/*
 * A284103  a(n) = Sum_{d|n, d == 4 (mod 5)} d.
 * https://oeis.org/A284103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{d|n, d ≡ 4 (mod 5)} d
 * Range:     u8 n <= 233, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;
            if (d1 % 5 == 4)
                sum += (seq_t)d1;
            if (d2 != d1 && d2 % 5 == 4)
                sum += (seq_t)d2;
        }
    }
    return sum;
}
