/*
 * A286927  Positions of 1 in A286925; complement of A286926.
 * https://oeis.org/A286927
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n + 2*floor(n*sqrt(2)) (= 2*floor(n*(2+sqrt(2))) )
 * Range:     u8 n <= 37, u16 n <= 9597, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(2)) by an incremental integer check. */
    idx_t k = 0;            /* floor(0*sqrt(2)) */
    for (idx_t i = 1; i <= n; ++i)
    {
        /* Increase k until (k+1)^2 > 2*i^2 */
        while (((unsigned __int128)(k + 1) * (k + 1)) <=
               (unsigned __int128)2 * i * i)
            ++k;
    }

    return 4 * (seq_t)n + 2 * (seq_t)k;
}
