/*
 * A386789  Triangle read by rows: T(n, k) = binomial(n - 1, k - 1)*binomial(n
 *          + k, k).
 * https://oeis.org/A386789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,c) where T(r,c) = binomial(r-1,c-1)*binomial(r+c,c);
 *            r = floor((sqrt(8*n+1)-1)/2), c = n - r*(r+1)/2 (with a(0)=1 as
 *            the only element of row 0)
 * Range:     u8 n <= 13, u16 n <= 43, u32 n <= 129, u64 n <= 423,
 *            u128 n <= 1518, big any n
 * Helpers:   binomial, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/seq_mul.h"
#include <stdbool.h>

seq_t a(idx_t n) {
    /* row 0 special */
    if (n == 0) return (seq_t)1;

    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    /* column index within the row */
    idx_t c = n - r * (r + 1) / 2;

    /* For n>0, only row >0 has entries; row 0 handled above. */
    if (c == 0) return (seq_t)0;           /* first element of any row r>=1 is 0 */

    /* Compute binomial values and their product */
    seq_t b1 = binomial(r - 1, c - 1);     /* binomial(r‑1, c‑1) */
    seq_t b2 = binomial(r + c, c);         /* binomial(r+c, c)   */
    return seq_mul(b1, b2);
}
