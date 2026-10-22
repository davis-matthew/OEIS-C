/*
 * A394362  Number of divisors d of n which also divide n/d + 1.
 * https://oeis.org/A394362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{ d | n : (n/d + 1) ≡ 0 (mod d) }
 * Range:     u8 n <= 806, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t count = 0;
    for (idx_t i = 1; i <= n / i; ++i)
    {
        if (n % i == 0)
        {
            seq_t d1 = (seq_t)i;
            seq_t m1 = ((seq_t)n) / d1;
            if ((m1 + 1) % d1 == 0)
                ++count;
            idx_t j = n / i;
            if (j != i)
            {
                seq_t d2 = (seq_t)j;
                seq_t m2 = ((seq_t)n) / d2;
                if ((m2 + 1) % d2 == 0)
                    ++count;
            }
        }
    }
    return count;
}
