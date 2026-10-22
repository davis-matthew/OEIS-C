/*
 * A355249  Maximal GCD of three positive integers with sum n.
 * https://oeis.org/A355249
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n / min{d | d | n, d >= 3}
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;   /* values below the offset are undefined */

    idx_t best = n;
    for (idx_t x = 2; x <= n / x; ++x)
    {
        if (n % x == 0)
        {
            if (x >= 3 && x < best)
                best = x;
            idx_t y = n / x;
            if (y >= 3 && y < best)
                best = y;
        }
    }

    return ((seq_t)n) / ((seq_t)best);
}
