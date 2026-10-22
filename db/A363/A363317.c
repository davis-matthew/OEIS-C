/*
 * A363317  Sum of divisors of 4*n-3 of form 4*k+1.
 * https://oeis.org/A363317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A050449(4*n-3) = sum of positive divisors d of (4*n-3)
 *            with d ≡ 1 (mod 4)
 * Range:     u8 n <= 56, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* m = 4*n - 3 */
    idx_t m = 4 * n;
    m -= 3;

    seq_t sum = 0;

    /* For every divisor d of m, add it if d ≡ 1 (mod 4) */
    for (idx_t d = 1; d <= m; ++d)
    {
        if ((m % d) == 0)
        {
            if ((d & 3) == 1)
                sum += (seq_t)d;
        }
    }

    return sum;
}
