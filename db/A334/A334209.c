/*
 * A334209  Number of solutions to n = i+j, 0 <= i,j <= n for which
 *          A010060(i)=A010060(j)=0, i != j.
 * https://oeis.org/A334209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = # {(i, j) | 0 <= i, j <= n, i + j = n, i != j, *
 *            popcount(i) even, popcount(j) even}
 * Range:     u8 n <= 638, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;          /* number of ordered pairs */
    idx_t i, j;

    for (i = 0; i <= n; ++i)
    {
        j = n - i;
        if (i == j)        /* exclude the i = j case */
            continue;

        /* popcount(i) % 2 == 0  &&  popcount(j) % 2 == 0 */
        if ( (__builtin_popcountll(i) & 1) == 0 &&
             (__builtin_popcountll(j) & 1) == 0 )
            ++cnt;
    }

    return cnt;
}
