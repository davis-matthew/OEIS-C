/*
 * A363805  Number of divisors of n of the form 7*k + 3.
 * https://oeis.org/A363805
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of divisors d of n with d ≡ 3 (mod 7)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            if ((d % 7) == 3)
                ++count;
            idx_t q = n / d;
            if (q != d && (q % 7) == 3)
                ++count;
        }
    }
    return count;
}
