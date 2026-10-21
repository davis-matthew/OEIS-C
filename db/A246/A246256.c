/*
 * A246256  Triangular array read by rows. Row n lists the coefficients of the
 *          closed form of hypergeometric([1/2, -n/2, (1-n)/2], [], 4*z).
 * https://oeis.org/A246256
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  tabf, nonn
 * Formula:   a(n) = value at row r, position k in the triangular array
 *            defined by T(r,k) = ∏_{j=0}^{⌊r/2⌋-k} (2*j-1)² * binomial(r, 2*k
 *            + (r mod 2)) where r is the smallest non‑negative integer such
 *            that n < Σ_{i=0}^{r} (⌊i/2⌋+1). The array is read by rows
 * Range:     u8 n <= 15, u16 n <= 24, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Product of the odd squares used in T(r,k). */
static inline seq_t tri_prod(idx_t r, idx_t k)
{
    idx_t m = r >> 1;          /* floor(r/2) */
    seq_t prod = 1;
    for (idx_t j = 0; j <= m - k; ++j)
    {
        int64_t odd = 2 * (int64_t)j - 1;   /* -1, 1, 3, ...  */
        seq_t factor = (seq_t)(odd * odd);  /* (2*j-1)^2  */
        prod *= factor;
    }
    return prod;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    idx_t pos = n;      /* position in the flattened array */
    idx_t r = 0;        /* current row */

    while (1)
    {
        idx_t row_len = (r >> 1) + 1;   /* length of row r */
        if (pos < row_len)
        {
            idx_t k = pos;           /* offset within the row */
            seq_t prod = tri_prod(r, k);
            idx_t bin_index = (r & 1) + 2 * k;  /* 2*k + (r mod 2) */
            seq_t bin = binomial(r, bin_index);
            return prod * bin;
        }
        pos -= row_len;
        ++r;
    }
}
