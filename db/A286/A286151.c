/*
 * A286151  Square array read by descending antidiagonals: If n > k, A(n,k) =
 *          T(n XOR k, k), and otherwise A(n,k) = T(n, n XOR k), where T(n,k)
 *          is sequence A001477 considered as a two-dimensional table, and XOR
 *          is bitwise-xor (A003987).
 * https://oeis.org/A286151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 126, u16 n <= 9168, u32 n <= 9168, u64 n <= 9168,
 *            u128 n <= 9168, big any n
 * Helpers:   sqrt_floor_128, table_term
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A286151: Square array read by descending antidiagonals.
   The 2‑D table T(n,k) that is read is given by the closed form
        T(n,k) = n*k + n*(n+1)/2 + k*(k+1)/2 + n
   The flat index n is converted to matrix coordinates (i,j) with
        s   = floor( (sqrt(8*n+1)-1)/2 )
        i   = n - s*(s+1)/2
        j   = s - i
   and a(n) is then
        a(n) = T(i xor j , j)   if i > j
               T(i , i xor j)   otherwise.  */

/* required types are defined by the build system */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/sqrt_floor_128.h"
#include "../utils/table_term.h"

/* helper: triangular number m*(m+1)/2, result fits in idx_t */
static idx_t triangular_idx(idx_t m)
{
    return (idx_t)(((unsigned __int128)m * (m + 1)) >> 1);
}

/* public function required by the template */
seq_t a(idx_t n)
{
    /* locate the antidiagonal: find s with s*(s+1)/2 <= n < (s+1)*(s+2)/2 */
    unsigned __int128 val = (unsigned __int128)n * 8 + 1;
    idx_t sqrt_val = sqrt_floor_128(val);
    idx_t s = (sqrt_val - 1) / 2;

    idx_t tri_s = triangular_idx(s);
    idx_t i = n - tri_s;       /* row index */
    idx_t j = s - i;           /* column index */

    idx_t xor_val = i ^ j;
    if (i > j)
        return table_term(xor_val, j);
    else
        return table_term(i, xor_val);
}
