/*
 * A324009  The number of convex polyominoes whose smallest bounding rectangle
 *          has size w*h (w > 0, h > 0). The table is read by antidiagonals.
 * https://oeis.org/A324009
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl, walk
 * Formula:   a(n) = number of convex polyominoes whose smallest bounding
 *            rectangle has size w×h, where (w,h) are the antidiagonal
 *            coordinates of n (offset 1). The value for each pair (w,h) is
 *            a(w,h) = binomial(2w+2h-4, 2w-2) + ((2w+2h-5)/2) *
 *            binomial(2w+2h-6, 2w-3) - 2*(w+h-3) * binomial(w+h-2, w-1) *
 *            binomi...
 * Range:     u8 n <= 12, u16 n <= 40, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* ---------- 1. Map index n to (w,h) via antidiagonals  ---------- */
    /* Find smallest s such that s*(s-1)/2 >= n   (sum = w + h)          */
    idx_t sum = 1;                         /* current sum candidate      */
    while ((sum * (sum - 1) / 2) < n) {
        ++sum;
    }
    /* Number of terms prior to this sum (for sum-1)                 */
    idx_t prior = (sum - 1) * (sum - 2) / 2;
    /* Zero‑based offset inside the current sum                     */
    idx_t offset = n - prior - 1;
    idx_t w = offset + 1;                  /* w ranges from 1 to sum-1  */
    idx_t h = sum - w;                     /* h automatically positive  */

    /* ---------- 2. Compute a(w,h) using the closed form ------------ */
    /* Term 1: C(2w+2h-4, 2w-2) */
    idx_t n1 = 2 * w + 2 * h - 4;
    idx_t k1 = 2 * w - 2;
    seq_t term1 = (k1 <= n1) ? binomial(n1, k1) : 0;

    /* Term 2: ((2w+2h-5)/2) * C(2w+2h-6, 2w-3)  */
    seq_t term2 = 0;
    if (sum >= 3) {                      /* ensures 2w+2h-6 >= 0 */
        idx_t n2 = 2 * w + 2 * h - 6;
        idx_t k2 = 2 * w - 3;
        if (k2 <= n2) {
            seq_t tmp = binomial(n2, k2);
            tmp *= (2 * sum - 5);         /* multiply by numerator        */
            term2 = tmp / 2;              /* exact division by 2          */
        }
    }

    /* Term 3: 2*(w+h-3) * C(w+h-2, w-1) * C(w+h-4, w-2)  */
    seq_t term3 = 0;
    if (sum >= 4 && w >= 2) {            /* guarantees all binomials valid */
        seq_t b1 = binomial(sum - 2, w - 1);
        seq_t b2 = binomial(sum - 4, w - 2);
        term3 = b1 * b2;
        term3 *= 2 * (sum - 3);
    }

    return term1 + term2 - term3;
}
