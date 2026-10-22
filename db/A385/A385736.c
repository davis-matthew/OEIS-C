/*
 * A385736  a(n) is the number of distinct nondegenerate triangles with
 *          perimeter n whose side lengths are triangular numbers.
 * https://oeis.org/A385736
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct nondegenerate triangles with perimeter
 *            n whose side lengths are triangular numbers
 * Range:     u8 n <= 7, u16 n <= 1978, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/floor_sqrt.h"

/* helper: triangular number k(k+1)/2 */
static inline seq_t triangular(idx_t k)
{
    return (seq_t)k * ((seq_t)k + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Perimeter too small to form a triangle with positive triangular sides. */
    if (n < 3)
        return 0;

    seq_t nseq = (seq_t)n;

    /* Compute the largest k with k(k+1)/2 <= n. */
    seq_t d       = 8 * nseq + 1;          /* discriminant 8n+1 */
    seq_t s       = floor_sqrt(d);          /* floor(sqrt(8n+1)) */
    seq_t kmax_t  = (s > 0) ? (s - 1) / 2 : 0;
    idx_t kmax    = (idx_t)kmax_t;

    seq_t result = 0;

    for (idx_t i = 1; i <= kmax; ++i) {
        seq_t a_len = triangular(i);
        if (a_len > nseq)
            break;

        for (idx_t j = i; j <= kmax; ++j) {
            seq_t b_len = triangular(j);
            if (b_len > nseq - a_len)
                break;

            seq_t ab = a_len + b_len;          /* a + b <= n, no overflow */
            seq_t c_len = nseq - ab;

            if (c_len < b_len)                 /* enforce a <= b <= c */
                continue;

            /* Triangle inequality: a + b > c */
            if (ab <= c_len)
                continue;

            /* Check if c is triangular: c = t(t+1)/2 */
            seq_t d_c = 8 * c_len + 1;
            seq_t s_c = floor_sqrt(d_c);
            if (s_c * s_c == d_c && (s_c & 1) == 1)
                ++result;
        }
    }

    return result;
}
