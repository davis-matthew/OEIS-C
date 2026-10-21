/*
 * A296350  List of numbers k such that the determinant of the Bordered Lights
 *          Out matrix BL_k is nonzero.
 * https://oeis.org/A296350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer m > 1 with (m+1) not divisible by 5 or 6
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t count = 0;
    seq_t m = 2;
    while (1)
    {
        seq_t k = m + 1;
        if ((k % 5) != 0 && (k % 6) != 0)
        {
            ++count;
            if (count == n)
                return m;
        }
        ++m;
    }
}
