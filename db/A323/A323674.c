/*
 * A323674  Square array, read by antidiagonals, of the positive integers 6cd
 *          +-c +-d = (6c +- 1)d +- c. Alternate rows (or columns) are numbers
 *          that differ by c from multiples of 6c - 1 or 6c + 1.
 * https://oeis.org/A323674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   find_sum, row_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Model: Square array read by antidiagonals
 * Formula (OEIS): a(m,n) = 6*floor((m+1)/2)*floor((n+1)/2)
 *                    + (-1)^n*floor((m+1)/2) + (-1)^m*floor((n+1)/2),
 * where (m,n) are the array indices (m,n ≥ 1).  
 * The sequence a(n) is obtained by traversing the array along
 * antidiagonals: for each sum s = m+n the entries (1,s-1),(2,s-2),…,(s-1,1)
 * are produced in that order.
 */

#include <stdint.h>
#include "../utils/find_sum.h"
#include "../utils/row_sum.h"

/* Implementation of A323674.  */
seq_t a(idx_t n)
{
    /* Map the 1‑based index n to array coordinates (m,n). */
    idx_t s   = find_sum(n);            /* sum m+n  */
    idx_t prev = row_sum(s - 1);        /* entries before this antidiagonal   */
    idx_t t   = n - prev;               /* position within the antidiagonal   */
    idx_t m   = t;                      /* m  ∈ [1, s-1]                      */
    idx_t n_  = s - t;                  /* n_∈ [1, s-1]                       */

    /* Helper integer values for the formula.  All calculations are
     * performed with signed 64‑bit arithmetic to avoid overflow
     * and to handle negative intermediate results reliably.   */
    int64_t cm   = (int64_t)(m + 1) / 2;                /* floor((m+1)/2)   */
    int64_t cn   = (int64_t)(n_ + 1) / 2;               /* floor((n+1)/2)   */
    int64_t sign1 = (n_ & 1) ? -1 : 1;                 /* (-1)^n            */
    int64_t sign2 = (m  & 1) ? -1 : 1;                 /* (-1)^m            */

    int64_t val = 6 * cm * cn + sign1 * cm + sign2 * cn;
    return (seq_t)val;
}
