/*
 * A331425  Divide each side of a triangle into 2*n (n>=1) equal parts and
 *          trace the corresponding cevians, i.e., join every point, except
 *          for the first and last ones, with the opposite vertex. a(n) is the
 *          number of points at which three cevians meet.
 * https://oeis.org/A331425
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A331423(2*n) The sequence counts the number of ordered
 *            triples (k,l,m) with 1 ≤ k,l,m ≤ 2n−1 satisfying
 *            (2n−k)(2n−l)(2n−m) = k·l·m
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Convert to 64‑bit integers for intermediate calculations.
       All values fit in 64 bits for n ≤ 10 000, but are cast to
       seq_t only at the final result. */
    uint64_t N = (uint64_t)n * 2ull;          /* N = 2n  */
    seq_t   count = 0;                         /* result */

    for (idx_t a = 1; a < N; ++a) {
        uint64_t na = N - (uint64_t)a;        /* N−a  */
        for (idx_t b = 1; b < N; ++b) {
            uint64_t nb = N - (uint64_t)b;    /* N−b  */

            /* num = N·(N−a)·(N−b)   */
            /* den = (N−a)(N−b) + a·b */
            uint64_t num = N * na * nb;
            uint64_t den = na * nb + (uint64_t)a * (uint64_t)b;

            if (den == 0) continue;           /* safety, never happens */
            if (num % den != 0) continue;     /* no integer c  */

            uint64_t c = num / den;
            if (c >= 1 && c < N) {
                ++count;                       /* seq_t increment is safe   */
            }
        }
    }

    return count;
}
