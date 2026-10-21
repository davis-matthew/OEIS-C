/*
 * A291572  The arithmetic function uhat(n,6,3).
 * https://oeis.org/A291572
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n (for n ≤ 7), * = 6 if 6|n, * = 7 if 7|n, * = 8 if 8|n,
 *            * = 8 if 3|n, * = 10 otherwise
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 7)
        return (seq_t)n;
    if (n % 6 == 0)
        return (seq_t)6;
    if (n % 7 == 0)
        return (seq_t)7;
    if (n % 8 == 0)
        return (seq_t)8;
    if (n % 3 == 0)
        return (seq_t)8;
    return (seq_t)10;
}
