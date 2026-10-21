/*
 * A271710  Table T(n,k) = 2^n XOR 2^k read by antidiagonals, where XOR is the
 *          binary exclusive or operator.
 * https://oeis.org/A271710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the value read by antidiagonals from table T(i,j) = 2^i
 *            XOR 2^j. For a given index n, find the diagonal d and offset
 *            along it: d = floor((sqrt(8n+1)-1)/2) offset = n - d(d+1)/2
 *            (i,j) = (d-offset, offset) Then a(n) = pow2(i) ^ pow2(j)
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 8255, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* Determine diagonal d */
    double tmp = sqrt(8.0 * (double)n + 1.0);
    idx_t d = (idx_t)((tmp - 1.0) / 2.0);

    /* Correct possible off‑by‑one due to floating‑point rounding */
    while ((d + 1) * (d + 2) / 2 <= n) ++d;
    while (d * (d + 1) / 2 > n) --d;

    /* Position on the diagonal */
    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = d - offset;
    idx_t j = offset;

    return pow2(i) ^ pow2(j);
}
