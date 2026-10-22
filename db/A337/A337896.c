/*
 * A337896  Number of chiral pairs of colorings of the 8 triangular faces of a
 *          regular octahedron or the 8 vertices of a cube using n or fewer
 *          colors.
 * https://oeis.org/A337896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*n^2*(n+1)*(8-5*n^2+n^4)/48
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 24, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses integer arithmetic only.  The result is computed as a product of
   four factors divided by 48, reducing a factor of 48 from each factor
   as it is encountered to avoid overflow.  The function works for
   any seq_t implementation (unsigned integer, big‑int class) and is
   O(1) in time. */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;          /* a(1) = 0 */

    seq_t N   = n;                       /* cast from idx_t to seq_t */
    seq_t N2  = N * N;                   /* N^2 */
    seq_t N4  = N2 * N2;                 /* N^4 */
    seq_t expr = N4 + (seq_t)8 - (seq_t)5 * N2;   /* 8 - 5N^2 + N^4 */

    /* factors of the numerator:
         (N-1), N^2, (N+1), expr
    */
    seq_t terms[4] = { N - (seq_t)1, N2, N + (seq_t)1, expr };
    seq_t rem   = (seq_t)48;             /* divisor to be removed */
    seq_t result = (seq_t)1;

    for (int i = 0; i < 4; ++i) {
        seq_t g = gcd(terms[i], rem);   /* common factor with the remaining divisor */
        if (g > 1) {
            rem /= g;                   /* reduce the remaining divisor */
            terms[i] /= g;              /* remove factor from current term */
        }
        result *= terms[i];
    }
    /* rem should now be 1; if not, divide the remaining part (in 64‑bit builds
       the remaining divisor is small (≤48) and will be handled by the last
       factor above).  The final division is an integer operation. */
    if (rem > 1) result /= rem;

    return result;
}
