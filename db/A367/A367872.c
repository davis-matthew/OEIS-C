/*
 * A367872  Number of dissections of a convex (4n+4)-sided polygon into n
 *          hexagons and one square (up to equivalence).
 * https://oeis.org/A367872
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(5*n+2,n)*(n+3)/(4*n+3)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute A367872:   Number of dissections of a (4n+4)-sided polygon
 *   into one square and n hexagons, up to the specified equivalence. */
seq_t a(idx_t n)
{
    /* Use 128‑bit arithmetic for intermediate products */
    seq_t bin = 1;                /* C(5n+2,n) will be built iteratively */
    idx_t N = 5*n + 2;            /* upper argument of the binomial */

    for (idx_t i = 1; i <= n; ++i) {
        /* Compute: bin = bin * (N - n + i) / i   */
        unsigned __int128 t = (unsigned __int128)bin *
                                 (N - (n - i));
        t /= i;                /* exact division */
        bin = (seq_t)t;
    }

    /* Multiply by (n+3) and divide by (4n+3) – the division is exact. */
    unsigned __int128 result = (unsigned __int128)bin * (n + 3);
    result /= (4*n + 3);

    return (seq_t)result;
}
