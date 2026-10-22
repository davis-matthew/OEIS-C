/*
 * A309332  Number of ways the n-th triangular number T(n) = A000217(n) can be
 *          written as the sum of two positive triangular numbers.
 * https://oeis.org/A309332
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unordered pairs (i,j) with i≥1, j≥1 and *
 *            T(i)+T(j)=T(n), where T(k)=k(k+1)/2
 * Range:     u8 n <= 3, u16 n <= 63, u32 n <= 9493, u64 n <= 9493,
 *            u128 n <= 9493, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t Tn = (seq_t)n * ((seq_t)n + 1) / 2;
    seq_t count = 0;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t Ti = (seq_t)i * ((seq_t)i + 1) / 2;
        seq_t S = Tn - Ti;          /* remaining part to be matched */
        if (S == 0) continue;       /* j must be positive */

        /* Solve j^2 + j - 2S = 0  →  j = (-1 + sqrt(1+8S))/2 */
        seq_t d = 1 + (seq_t)8 * S;
        seq_t sqrt_d = floor_sqrt(d);
        if (sqrt_d * sqrt_d != d) continue;     /* S is not triangular */

        seq_t j = (sqrt_d - 1) / 2;
        if ((idx_t)j < i) continue;   /* enforce i ≤ j to avoid duplicates */
        if ((idx_t)j == 0) continue;   /* j must be positive */

        /* Safety check: verify that T(j) equals S */
        seq_t Tj = (seq_t)j * ((seq_t)j + 1) / 2;
        if (Tj != S) continue;

        count++;
    }
    return count;
}
