/*
 * A378704  Array read by ascending antidiagonals: A(n, k) is the total area
 *          of n-Fibonacci polyominoes with k columns, where k > 0.
 * https://oeis.org/A378704
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the array read by ascending antidiagonals of the
 *            triangle A(n,k) defined by the generating function A(n, k) =
 *            [y^k] y*(n^2*(1 - y)^2*y^n + 2*y*(1 - y^n) - n(1 - y)*(2- y^n +
 *            y^(n+1)))/(2*(-1 + y)*(1 - 2*y + y^(n+1))^2). The function below
 *            computes a(n) directly from this definition
 * Range:     u8 n <= 20, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   compute_A_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/compute_A_nk.h"

/* Public function: a(n) for the OEIS sequence A378704. */
seq_t a(idx_t n)
{
    if (n < 2)
        return 0;              /* behaviour below offset is unspecified */

    idx_t idx = n - 2;          /* zero‑based index in the anti‑diagonal order */

    /* Find the antidiagonal d such that the cumulative number of terms
       up to d-1 is < idx and up to d is ≥ idx+1.  Here d = current sum n+k. */
    idx_t d = 3;
    while ((d - 2) * (d - 1) / 2 <= idx)
        ++d;

    idx_t prev_cum = (d - 3) * (d - 2) / 2;   /* terms before diagonal d */
    idx_t offset = idx - prev_cum;            /* position within diagonal d */

    idx_t row = (d - 1) - offset;   /* value of n in A(n,k) */
    idx_t col = offset + 1;         /* value of k in A(n,k) */

    return compute_A_nk(row, col);
}
