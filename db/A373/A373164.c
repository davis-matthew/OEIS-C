/*
 * A373164  Triangle read by rows: the exponential almost-Riordan array ( 1 |
 *          2 - exp(x), x ).
 * https://oeis.org/A373164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n-th element (row‑major) of the exponential
 *            almost‑Riordan triangle T, where T(r,0) = 1 if r==0 else 0,
 *            T(r,r) = 1, T(r,k) = - C(r-1, k-1) for 0<k<r. (C is the binomial
 *            coefficient)
 * Range:     i8 n <= 65, i16 n <= 65, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Convert a global index n into row r and column c of the triangle. */
static void _index_to_rc(idx_t n, idx_t *r, idx_t *c)
{
    if (n == 0) {
        *r = 0;
        *c = 0;
        return;
    }
    idx_t sum = 1;   /* number of elements up to current row r */
    *r = 0;
    while (n >= sum) {
        ++(*r);
        sum += (*r) + 1;
    }
    *c = n - (sum - (*r + 1));
}

seq_t a(idx_t n)
{
    idx_t r, c;
    _index_to_rc(n, &r, &c);

    if (c == 0) {
        /* T(r,0) = 1 if r==0, otherwise 0 */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }
    if (c == r) {
        return (seq_t)1;
    }
    /* For 0 < c < r, use the binomial formula with a minus sign */
    seq_t bin = binomial(r - 1, c - 1);
    return -bin;
}
