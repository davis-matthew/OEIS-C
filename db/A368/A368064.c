/*
 * A368064  a(n) = Product_{i=1..n, j=1..n} (i^2 + 4*i*j + j^2).
 * https://oeis.org/A368064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{i=1..n} ∏_{j=1..n} (i^2 + 4*i*j + j^2) (offset 0)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t ii = (seq_t)i;
        seq_t ii2 = ii * ii;
        for (idx_t j = 1; j <= n; ++j)
        {
            seq_t jj = (seq_t)j;
            seq_t val = ii2 + (ii * jj * (seq_t)4) + jj * jj;
            result *= val;
        }
    }
    return result;
}
