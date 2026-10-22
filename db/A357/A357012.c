/*
 * A357012  Triangle read by rows. T(n, k) = [x^k](0^n + 4^n * ((2 - 2*(1 -
 *          x)^(1/2)) / x - 1)).
 * https://oeis.org/A357012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(row,col) where T(n,k) = 0^n if k=0, Catalan(n) if k=n,
 *            otherwise 4^(n-k)*Catalan(k). The sequence is the triangle read
 *            by rows
 * Range:     u8 n <= 15, u16 n <= 45, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   catalan, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow4.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Find row r such that cumulative terms up to row r-1 ≤ n < cumulative terms up to row r */
    idx_t r = 0;
    /* Use 128‑bit arithmetic to avoid overflow for very large n */
    while (1) {
        unsigned __int128 cum = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (cum > (unsigned __int128)n)
            break;
        r++;
    }

    /* Base index of the start of this row */
    unsigned __int128 base128 = (unsigned __int128)r * (r + 1) / 2;
    idx_t base = (idx_t)base128;

    idx_t c = n - base;   /* column within the row */

    if (c == 0) {                 /* 0^r: 1 if r==0 else 0 */
        return (r == 0) ? (seq_t)1 : (seq_t)0;
    }

    if (c == r) {                  /* last element is a Catalan number */
        return catalan(r);
    }

    /* General case: T(r, c) = 4^(r-c) * Catalan(c) */
    seq_t pow_val = pow4(r - c);
    seq_t cat_val = catalan(c);

    return pow_val * cat_val;
}
