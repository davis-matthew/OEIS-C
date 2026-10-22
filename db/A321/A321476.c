/*
 * A321476  Regular triangle read by rows: T(n,k) is the rank of
 *          {A172236(k,m)} modulo n, 0 <= k <= n - 1.
 * https://oeis.org/A321476
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   rank_mod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   This function implements OEIS A321476.
   The sequence is read by rows of the triangle T(n,k),
   where for each n ≥ 1 and 0 ≤ k ≤ n−1,
   T(n,k) is the smallest positive integer m with
   A172236(k,m) ≡ 0 (mod n).  
   The sequence A172236(k,m) is the generalized Fibonacci
   (Lucas) sequence with recurrence
        U_0 = 0,  U_1 = 1,
        U_m = k·U_{m-1} + U_{m-2}   (m ≥ 2).
   The function computes a(n) by finding the (row,column) pair
   for the global index n and then simulating the recurrence
   mod the row number until a zero occurs.
*/

#include <stdint.h>
#include <stdbool.h>
#include "../utils/rank_mod.h"

/* Main function required by the problem statement. */
seq_t a(idx_t idx)
{
    /* Find the row r such that r*(r+1)/2 >= idx. */
    idx_t r = 1;
    while (r * (r + 1) / 2 < idx)
        ++r;

    /* Number of terms before this row. */
    idx_t prev_total = r * (r - 1) / 2;

    /* Column within the row (0‑based). */
    idx_t k = idx - prev_total - 1;

    /* Return the rank T(r,k). */
    return rank_mod(r, k);
}
