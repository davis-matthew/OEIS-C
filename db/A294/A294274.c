/*
 * A294274  Sum of the seventh powers of the parts in the partitions of n into
 *          two parts.
 * https://oeis.org/A294274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..floor(n/2)} i^7 + (n-i)^7 The implementation
 *            loops over the floor(n/2) terms, computing each seventh power in
 *            128‑bit arithmetic to avoid overflow between multiplication
 *            steps, then casts the result back to seq_t
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 21, u64 n <= 332, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor(n/2) gives the number of distinct partitions (unordered) */
    idx_t m = n / 2;
    unsigned __int128 sum = 0;

    for (idx_t i = 1; i <= m; ++i)
    {
        unsigned __int128 ii = (unsigned __int128)i;
        unsigned __int128 jj = (unsigned __int128)(n - i);

        /* compute i^7 and (n-i)^7 */
        unsigned __int128 i7 = ii * ii * ii * ii * ii * ii * ii;
        unsigned __int128 j7 = jj * jj * jj * jj * jj * jj * jj;

        sum += i7 + j7;
    }

    return (seq_t)sum;
}
