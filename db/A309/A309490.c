/*
 * A309490  Total number of adjacent node merge operations to turn a circular
 *          list of size n to a node.
 * https://oeis.org/A309490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + n*a(n-1) for n >= 3, a(1) = 0, a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the defined offset 1..                                    */
    if (n < 1)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;

    /* Compute iteratively:  a(2) = 1, then for i = 3..n
       a(i) = i + i * a(i-1).                                     */
    seq_t val = (seq_t)1;               /* a(2) */
    for (idx_t i = 3; i <= n; ++i)
        val = (seq_t)i + (seq_t)i * val;  /* a(i) */

    return val;                        /* a(n) */
}
