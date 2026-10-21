/*
 * A278147  Triangle read by rows of Cantor pairing function value determining
 *          primitive Pythagorean triangles or 0 if there is no such triangle.
 * https://oeis.org/A278147
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Computes a(n) of OEIS A278147.
   Consider the triangular array with rows r = 2,3,... and
   columns m = 1,...,r-1.  The index n (with offset 2) enumerates the
   entries row‑by‑row.  For a given n we recover the corresponding
   (row, col) pair, then return
        0                                        if GCD(row,col) > 1 or (row+col) even,
        (row+col)*(row+col+1)/2 + row            otherwise.
   (row+col)*(row+col+1)/2 + row is the Cantor pairing of (col,row).
*/

#include <stdint.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 2) return 0;                 /* undefined for n < 2 */

    /* Recover the row and column from the index */
    idx_t t   = n - 1;                   /* 1‑based position within the
                                            triangular array */
    idx_t r   = 2;                       /* start from the first row  */
    /* find smallest r such that r*(r-1)/2 >= t */
    while ((r * (r - 1) / 2) < t) r++;

    /* number of entries before this row */
    idx_t cum_prev = (r - 2) * (r - 1) / 2;
    idx_t m       = t - cum_prev;        /* 1‑based column index */

    if ( ((r + m) & 1) == 0 ) return 0;  /* even sum => not primitive */

    /* GCD condition */
    if ( gcd((seq_t)r, (seq_t)m) != 1 ) return 0;

    /* Cantor pairing value */
    uint64_t sum   = (uint64_t)r + (uint64_t)m;
    uint64_t cantor = sum * (sum + 1) / 2 + (uint64_t)r;
    return (seq_t)cantor;
}
