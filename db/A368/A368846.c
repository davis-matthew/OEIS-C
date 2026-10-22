/*
 * A368846  Triangle read by rows: T(n, k) = (-1)^(n + k)*2*binomial(2*k - 1,
 *          n)* binomial(2*n + 1, 2*k) for k > 0, and k^n for k = 0.
 * https://oeis.org/A368846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2; T(r,0) = (r==0?1:0); T(r,k>0) = (-1)^(r+k) * 2 *
 *            binomial(2*k-1, r) * binomial(2*r+1, 2*k)
 * Range:     i8 n <= 8, i16 n <= 25, i32 n <= 86, i64 n <= 291,
 *            i128 n <= 1065, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    /* determine row r */
    idx_t r;
    double d = sqrt((double) (8ULL * n + 1ULL));
    r = (idx_t) ((d - 1.0) / 2.0);

    /* adjust r in case of round‑off */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r > 0 && r * (r + 1) / 2 > n) --r;

    idx_t k = n - r * (r + 1) / 2;   /* column index */

    if (k == 0) {
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    seq_t a1 = binomial(2 * k - 1, r);
    seq_t a2 = binomial(2 * r + 1, 2 * k);
    seq_t val = a1 * a2;
    val *= 2;

    if (((r + k) & 1) != 0) {
        val = -val;
    }
    return val;
}
