/*
 * A320015  Number of proper divisors of n that are either of the form 6*k+1
 *          or 6*k + 5.
 * https://oeis.org/A320015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the number of proper divisors d of n with d ≡ 1 or 5 (mod
 *            6)
 * Range:     u8 n <= 9337, u16 n <= 9337, u32 n <= 9337, u64 n <= 9337,
 *            u128 n <= 9337, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n <= 1 gives no proper divisor */
    if (n <= 1)
    {
        return 0;
    }

    seq_t cnt = 0;
    idx_t i;
    for (i = 1; i * i <= n; ++i)
    {
        if (n % i == 0)
        {
            idx_t d1 = i;
            idx_t d2 = n / i;

            /* d1 is always <= sqrt(n), so d1 < n automatically
               except when n == 1 which is handled above */
            if (d1 < n && (d1 % 6 == 1 || d1 % 6 == 5))
            {
                ++cnt;
            }

            /* d2 can be equal to d1 when n is a square; skip duplicates */
            if (d2 != d1 && d2 < n && (d2 % 6 == 1 || d2 % 6 == 5))
            {
                ++cnt;
            }
        }
    }
    return cnt;
}
