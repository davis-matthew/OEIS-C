/*
 * A283810  Numbers of variables for which the Shapiro inequality holds.
 * https://oeis.org/A283810
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, full, fini
 * Formula:   a(n) is the nth integer k ≥ 3 such that k ≤ 23 and * (k is odd
 *            or k is even and k ≤ 12). This describes the known * values for
 *            which the Shapiro inequality holds for all positive * arguments.
 *            The sequence is finite; for n larger than its length * the
 *            function returns 0
 * Range:     u8 n <= 16, u16 n <= 16, u32 n <= 16, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t cnt = 0;
    /* Scan the possible k in increasing order. */
    for (idx_t k = 3; k <= 23; ++k)
    {
        /* Even numbers are allowed only up to 12. */
        if ((k & 1) || (k <= 12))
        {
            ++cnt;
            if (cnt == n)
                return (seq_t)k;
        }
    }
    /* n exceeds the length of the sequence */
    return 0;
}
