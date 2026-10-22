/*
 * A351782  a(n) = n-2 if n is a multiple of 4, a(n) = n-1 otherwise.
 * https://oeis.org/A351782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n-2 if n mod 4 = 0, otherwise a(n) = n-1
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Offset: 1  (a(1) = 0) */
seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 3ULL) == 0ULL)   /* n mod 4 == 0 */
        return N - 2;
    else
        return N - 1;
}
