/*
 * A355245  Square array A(n, k), n, k >= 0, read by antidiagonals; for any m
 *          > 0, the position of the m-th rightmost 0 in the binary expansion
 *          of A(n, k) is the least of the positions of the m-th rightmost 0
 *          in the binary expansions of n and k (the least significant bit
 *          having position 0).
 * https://oeis.org/A355245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabl
 * Formula:   a(n) is the entry A(i,j) of the square array A(n,k) read by
 *            antidiagonals, where i,j are obtained from n by the function
 *            antidiagonal_coords. The entry A(i,j) is defined by the
 *            positions of the rightmost zeros in i and j: an m‑th zero of A
 *            is at the least position among the m‑th zeros of i and j....
 * Range:     u8 n <= 10010, u16 n <= 10010, u32 n <= 10010, u64 n <= 10010,
 *            u128 n <= 10010, big any n
 * Validated: standard, pass, 107 terms through n = 9500 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

/* mapping from linear index n to pair (i,j) on the antidiagonals of T */
static inline void antidiagonal_coords(idx_t n, idx_t *i, idx_t *j)
{
    /* find s such that (s+1)(s+2)/2 <= n < (s+2)(s+3)/2 */
    idx_t s = 0;
    while (((s + 1) * (s + 2)) / 2 <= n) ++s;
    idx_t pos = n - (s * (s + 1)) / 2;  /* 0 <= pos <= s */
    *i = pos;
    *j = s - pos;
}
#include <stdbool.h>

/* Compute A(n,k) according to the zero‑position rule described above.
   n and k are the (non‑negative) row and column indices.  The result
   fits into the seq_t type (unsigned integer of the appropriate width). */
static seq_t A_nk(idx_t n, idx_t k)
{
    seq_t a = 0;
    idx_t n_local = n;
    idx_t k_local = k;
    idx_t cnt_n = 0;      /* zeros counted in n below current bit */
    idx_t cnt_k = 0;      /* zeros counted in k below current bit */
    idx_t pos   = 0;      /* current bit position */

    /* Process bits until both numbers are exhausted.  Higher bits
       are all zero anyway, and do not affect the value of a. */
    while (n_local || k_local) {
        bool zero_n = ((n_local & 1ULL) == 0);
        bool zero_k = ((k_local & 1ULL) == 0);

        if (zero_n && zero_k) {
            /* A has a zero at this bit, increment both zero counters. */
            cnt_n++;
            cnt_k++;
        } else if (zero_n) {          /* only n has zero at this bit */
            if (cnt_k <= cnt_n) {
                /* Zero, because k's current zero count is less or equal,
                   so k's next zero is not beyond this position. */
            } else {
                a |= pow2(pos);       /* set the bit to 1 */
            }
            cnt_n++;
        } else if (zero_k) {          /* only k has zero at this bit */
            if (cnt_n <= cnt_k) {
                /* Zero for the same reason as above. */
            } else {
                a |= pow2(pos);       /* set the bit to 1 */
            }
            cnt_k++;
        } else {                    /* both bits are 1 */
            a |= pow2(pos);
        }

        pos++;
        n_local >>= 1;
        k_local >>= 1;
    }
    return a;
}

/* Public sequence function expected by the build system. */
seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);
    return A_nk(i, j);
}
