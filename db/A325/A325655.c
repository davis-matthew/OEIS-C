/*
 * A325655  Triangle read by rows: T(n, k) = (1/4)*(2*(-1 + (-1)^n)*k -
 *          2*k^2*n + n*(2 - (-1)^k - (-1)^n + 2*n^2)), with 0 <= k < n.
 * https://oeis.org/A325655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the nth entry of the triangle read by rows T(n,k) =
 *            (1/4)*(2*(-1 + (-1)^n)*k - 2*k^2*n + n*(2 - (-1)^k - (-1)^n +
 *            2*n^2)) with 1 ≤ n and 0 ≤ k < n. The sequence is obtained by
 *            flattening this triangle row by row: a(1)=T(1,0), a(2)=T(2,0),
 *            a(3)=T(2,1), a(4)=T(3,0), …
 * Range:     u8 n <= 7, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   floor_sqrt, triangle_value
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"
#include "../utils/triangle_value.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≥ n >
       (r‑1)*r/2 < n.  n ≥ 1. */
    seq_t t = (seq_t)8 * (seq_t)n + 1;   /* 8*n + 1 */
    seq_t s = floor_sqrt(t);            /* floor(sqrt(8*n+1)) */
    idx_t r = (idx_t)((s - 1) / 2);      /* initial guess */

    /* Adjust r upward if necessary (due to floor sqrt) */
    seq_t curr = (seq_t)r * ((seq_t)r + 1) / 2;
    while (curr < (seq_t)n) {
        r++;
        curr = (seq_t)r * ((seq_t)r + 1) / 2;
    }

    /* Previous total terminates at row r‑1 */
    seq_t prev = ((seq_t)(r - 1)) * (seq_t)r / 2;

    /* Index within the row (0‑based) */
    idx_t k = (idx_t)(((seq_t)n - prev) - 1);

    return triangle_value(r, k);
}
