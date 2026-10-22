/*
 * A380245  a(n) = A378684(A379343(n)).
 * https://oeis.org/A380245
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = S(r-1) + P(r, p) where r is the smallest integer such
 *            that r(2r-1) ≥ n, p = n – S(r-1) (1‑based position within the
 *            row), S(t) = t(2t-1) (sum of the first t row lengths), P(r,p) is
 *            defined as in the OEIS entry: let m = 2r-1 if (p < m) { if (p
 *            odd) P = 2m - p - 1; else P = m - p; } else { i...
 * Range:     u8 n <= 231, u16 n <= 9730, u32 n <= 9730, u64 n <= 9730,
 *            u128 n <= 9730, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for the offset – return 0 as a safe value */
        return (seq_t)0;

    /* find the row r */
    idx_t r = 1;
    while ( ((__uint128_t)r * (2 * r - 1)) < n )
        r++;

    /* position within the row */
    unsigned __int128 tmp = (__uint128_t)(r - 1) * (2 * r - 3);  /* S(r-1) */
    idx_t prev_total = (idx_t)tmp;                    /* numbers before this row */
    idx_t p = n - prev_total;                         /* 1‑based index in the row */

    /* compute P(r, p) */
    idx_t m = 2 * r - 1;
    idx_t P;
    if (p < m) {
        if (p & 1) { /* odd */
            P = 2 * m - p - 1;
        } else {     /* even */
            P = m - p;
        }
    } else {
        if (p & 1) { /* odd */
            P = p;
        } else {     /* even */
            P = p - m + 1;
        }
    }

    /* final value */
    return (seq_t)(prev_total + P);
}
