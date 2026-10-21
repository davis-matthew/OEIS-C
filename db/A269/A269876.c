/*
 * A269876  Number of active (ON, black) cells in n-th stage of growth of
 *          two-dimensional cellular automaton defined by "Rule 43", based on
 *          the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269876
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2+2n+2)/2 for n even; a(n) = (7n^2+4n-1)/2 for n odd
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use a 128‑bit unsigned integer for intermediate calculations to avoid overflow. */
    unsigned __int128 N = n;
    if ((n & 1ULL) == 0) {                     /* even n */
        unsigned __int128 v = (N * N + 2 * N + 2) >> 1; /* divide by 2 via shift */
        return (seq_t)v;
    } else {                                   /* odd n */
        unsigned __int128 v = (7 * N * N + 4 * N - 1) >> 1;
        return (seq_t)v;
    }
}
