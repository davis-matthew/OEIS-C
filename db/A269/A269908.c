/*
 * A269908  Partial sums of the number of active (ON, black) cells in n-th
 *          stage of growth of two-dimensional cellular automaton defined by
 *          "Rule 1", based on the 5-celled von Neumann neighborhood.
 * https://oeis.org/A269908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n^3 + 6*n^2 - 13*n + 6)/6 if n is even a(n) = (4*n^3 +
 *            18*n^2 + 11*n - 3)/6 if n is odd
 * Range:     u8 n <= 6, u16 n <= 44, u32 n <= 128, u64 n <= 128,
 *            u128 n <= 128, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use 64‑bit signed arithmetic for the intermediate range [-13*n, 4*n^3] */
    long long t   = (long long)n;
    long long term, val;

    if ((n & 1ULL) == 0)      /* even n */
    {
        term = 4 * t * t * t + 6 * t * t - 13 * t + 6;
        val = term / 6;
    }
    else                     /* odd n */
    {
        term = 4 * t * t * t + 18 * t * t + 11 * t - 3;
        val = term / 6;
    }

    return (seq_t)val;
}
