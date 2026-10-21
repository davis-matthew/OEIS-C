/*
 * A278390  Triangle T(n,k) read by rows: the number of independent sets of
 *          size k in the 132 core of size n.
 * https://oeis.org/A278390
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, tabl, nonn
 * Formula:   a(n) is the flattened triangle T(r,k) read by rows. The triangle
 *            entry T(r,k) (0‑based) satisfies r*T(r,k) = Σ_{j=0}^{r-1} C(r,
 *            k-j) * C(r, j+1) * C(r-1+j, r-1) where C(n,k) is the binomial
 *            coefficient. The integer r is the row that contains a(n) and is
 *            the smallest index such that (r+1)(r+2)/2 ≥...
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* Find the row r (0‑based) containing a(n).  */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < n) {
        r++;
    }

    /* Previous cumulative count of elements up to row r‑1.  */
    idx_t prev = r * (r + 1) / 2;
    /* Offset within the row (0‑based).  */
    idx_t k = n - prev - 1;

    /* Special case: row 0 has only one element T(0,0)=1.  */
    if (r == 0) {
        return 1;
    }

    /* Compute T(r,k) using the given formula.  */
    seq_t sum = 0;
    for (idx_t j = 0; j < r; j++) {
        /* k-j must be in [0, r] for the binomial to be non‑zero. */
        if (k < j) continue;
        idx_t kj = k - j;
        if (kj > r) continue;

        seq_t term1 = binomial(r, kj);          /* C(r, k-j) */
        seq_t term2 = binomial(r, j + 1);       /* C(r, j+1) */
        seq_t term3 = binomial(r - 1 + j, r - 1); /* C(r-1+j, r-1) */

        seq_t prod = term1;
        prod *= term2;
        prod *= term3;
        sum += prod;
    }

    /* Divide by r to obtain T(r,k).  */
    seq_t r_seq = r;
    sum /= r_seq;
    return sum;
}
