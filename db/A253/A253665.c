/*
 * A253665  a(n) = 2^n*n!/(floor(n/2)!)^2.
 * https://oeis.org/A253665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n * n! / (floor(n/2)!)^2 Recurrence: a(0) = 1 for i >
 *            0: if i is odd: a(i) = a(i-1) * (4*((i-1)/2)+2) if i is even:
 *            a(i) = a(i-1) / (i/2) * 4 // division is exact
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    seq_t res = (seq_t)1;          /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
    {
        if ((i & (idx_t)1) == 0)          /* even i : i = 2k */
        {
            idx_t k = i >> 1;             /* k = i/2 */
            res = res / (seq_t)k;         /* exact division */
            res = res * (seq_t)4;
        }
        else                              /* odd i : i = 2k+1 */
        {
            idx_t k = (i - 1) >> 1;        /* k = (i-1)/2 */
            res = res * (seq_t)(4 * k + 2);
        }
    }
    return res;
}
