/*
 * A273831  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 961", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A273831
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n = 0 * 4 if n = 1 * 21 if n = 2 * 4*(n^2+n-1) if n
 *            is even and n > 2 * 4*n^2+4*n-3 if n is odd and n > 2
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the first three special values */
    if (n == 0U) return (seq_t)1;
    if (n == 1U) return (seq_t)4;
    if (n == 2U) return (seq_t)21;

    /* use an unsigned 64‑bit type to prevent overflow of the
       intermediate arithmetic.  n is at most ~10^4 in the
       tests, so unsigned long long is ample. */
    unsigned long long N = (unsigned long long)n;

    if ((n & 1ULL) == 0ULL)       /* even n */
    {
        unsigned long long val = 4ULL * (N * N + N - 1ULL);
        return (seq_t)val;
    }
    else                          /* odd n */
    {
        unsigned long long val = 4ULL * N * N + 4ULL * N - 3ULL;
        return (seq_t)val;
    }
}
