/*
 * A273983  a(n) = ((4*n)!! - (4*n-1)!!)/(4*n+1).
 * https://oeis.org/A273983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ((4*n)!! - (4*n-1)!!)/(4*n+1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t even = 1;
    seq_t odd  = 1;

    for (idx_t k = 1; k <= 2 * n; ++k)
    {
        even *= (seq_t)(2 * k);
        odd  *= (seq_t)(2 * k - 1);
    }

    seq_t num = even;
    num -= odd;
    seq_t res = num / (seq_t)(4 * n + 1);
    return res;
}
