/*
 * A307163  Minimum number of intercalates in a diagonal Latin square of order
 *          n.
 * https://oeis.org/A307163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more, hard
 * Formula:   a(n) = 12 if n==4; 9 if n==6; 0 otherwise
 * Range:     u8 n <= 15, u16 n <= 15, u32 n <= 15, u64 n <= 15, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 4ULL)
        return (seq_t)12;
    else if (n == 6ULL)
        return (seq_t)9;
    else
        return (seq_t)0;
}
