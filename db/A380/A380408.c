/*
 * A380408  a(n) = Sum_{k>=0} floor(n/(2k)!).
 * https://oeis.org/A380408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=0} floor(n/(2k)!)
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; ; ++k)
    {
        seq_t f = 1;
        for (idx_t i = 2; i <= 2 * k; ++i)
            f *= i;
        if (f > (seq_t)n)
            break;
        sum += n / f;
    }
    return sum;
}
