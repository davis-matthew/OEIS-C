/*
 * A279724  Transpose of array A257943.
 * https://oeis.org/A279724
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 25, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   pow_of_three
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Transpose of array A257943 – OEIS A279724 */
/* a(n) is obtained by enumerating A257943 by anti‑diagonals:
   let t be the smallest integer with t*(t+1)/2 ≥ n,
   let offset = n - (t-1)*t/2 - 1  (0‑based position in the t‑th diagonal),
   then i = t - offset   (row index),
         j = offset + 1 (column index).
   Finally a(n) = (1 + 3^(j-1)*(2*i - 1)) / 2.  */

#include <stdint.h>
#include <math.h>
#include "../utils/pow_of_three.h"

seq_t a(idx_t n)
{
    /* n ≥ 1 for the OEIS offset */
    uint64_t N = (uint64_t)n;

    /* Rough estimate of the diagonal number t */
    uint64_t t = (uint64_t)((sqrt(8.0 * (double)N + 1.0) - 1.0) / 2.0);

    /* Correct the estimate */
    while ((unsigned __int128)t * (t + 1) / 2 < N) ++t;
    while (t > 0 && (unsigned __int128)(t - 1) * t / 2 >= N) --t;

    uint64_t prev   = (t - 1) * t / 2;
    uint64_t offset = N - prev - 1;          /* 0‑based index within the diagonal */
    uint64_t j      = offset + 1;            /* column index (1‑based)   */
    uint64_t i      = t - offset;            /* row    index (1‑based)   */

    seq_t pow3 = pow_of_three(j - 1);          /* 3^(j-1) */
    seq_t two_i_minus1 = (seq_t)2 * (seq_t)i - (seq_t)1;

    seq_t prod = pow3 * two_i_minus1;
    seq_t result = (prod + (seq_t)1) / (seq_t)2;

    return result;
}
