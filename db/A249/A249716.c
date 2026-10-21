/*
 * A249716  The least nonsquarefree number on row n of Pascal's triangle, or 1
 *          if all the terms on that row are squarefree.
 * https://oeis.org/A249716
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least nonsquarefree binomial coefficient on row n, or 1
 *            if all binomial coefficients on that row are squarefree
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 46, u64 n <= 142,
 *            u128 n <= 670, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Binomial coefficient C(n,0) = 1, which is squarefree.  */
    seq_t c = (seq_t)1;

    /* Check coefficients up to the middle of the row – symmetry
       ensures the first non‑squarefree one occurs here if it exists. */
    for (idx_t k = 1; k <= n / 2; ++k) {
        c = c * (seq_t)(n - k + 1) / k; /* C(n,k) from C(n,k-1) */
        if (!is_squarefree(c))
            return c; /* first nonsquarefree coefficient found */
    }

    /* All coefficients on this row are squarefree. */
    return (seq_t)1;
}
