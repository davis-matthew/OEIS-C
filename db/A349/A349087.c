/*
 * A349087  a(n) = n! * Sum_{k=0..floor(n/4)} (-1)^k / (4*k)!.
 * https://oeis.org/A349087
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Sum_{k=0..floor(n/4)} (-1)^k / (4*k)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 4;
    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= m; ++k)
    {
        seq_t term = (seq_t)1;
        idx_t start = 4 * k + 1;
        for (idx_t i = start; i <= n; ++i)
            term *= (seq_t)i;    /* term = n! / (4k)! */

        if ((k & 1) == 0)
            result += term;
        else
            result -= term;
    }

    return result;
}
