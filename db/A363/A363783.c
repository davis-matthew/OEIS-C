/*
 * A363783  L.g.f.: log( Sum_{k>=0} x^(k^3) ).
 * https://oeis.org/A363783
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^(n-1) in S'(x)/S(x), * where S(x) =
 *            Sum_{k>=0} x^(k^3). Equivalent to * a(n) = n times coefficient
 *            of x^n in log( Sum_{k>=0} x^(k^3) )
 * Range:     i8 n <= 31, i16 n <= 66, i32 n <= 137, i64 n <= 285,
 *            i128 n <= 575, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* sequence starts at index 1 */;
    idx_t N = n;                    /* we need terms up to index n-1 */

    /* determine all cubes <= N */
    size_t cubeCount = 0;
    for (idx_t p = 1; ; ++p) {
        idx_t c = p * p * p;
        if (c > N) break;
        ++cubeCount;
    }
    idx_t *cubes = (idx_t*)malloc(cubeCount * sizeof(idx_t));
    for (idx_t p = 1; p <= cubeCount; ++p) {
        cubes[p - 1] = p * p * p;
    }

    /* Q[k] will hold coeff of x^k in S'(x)/S(x), 0 <= k < N */
    seq_t *Q = (seq_t*)malloc(N * sizeof(seq_t));
    if (!Q) { free(cubes); return 0; }

    for (idx_t k = 0; k < N; ++k) {
        /* coefficient of x^k in S'(x) */
        seq_t a_coeff = 0;
        for (size_t ci = 0; ci < cubeCount; ++ci) {
            idx_t c = cubes[ci];
            if (c == k + 1) { a_coeff = (seq_t)c; break; }
            if (c > k + 1) break;
        }

        /* subtract contributions from previous Q values */
        seq_t sum = 0;
        for (size_t ci = 0; ci < cubeCount; ++ci) {
            idx_t e = cubes[ci];
            if (e > k) break;
            sum += Q[k - e];
        }

        Q[k] = a_coeff - sum;
    }

    seq_t result = Q[n - 1];
    free(Q);
    free(cubes);
    return result;
}
