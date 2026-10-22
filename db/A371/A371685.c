/*
 * A371685  Triangle read by rows: T(n, k) = n! * Sum_{j=0..n-1} binomial(k -
 *          1, j) / (j + 1).
 * https://oeis.org/A371685
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth element of triangle A371685. For the row r and
 *            0‑based column k such that r(r+1)/2 + k = n: if k == 0: T(r,0) =
 *            r! * Σ_{j=0}^{r-1} (-1)^j / (j+1) otherwise: T(r,k) = r! * (2^k
 *            – 1) / k (integer division)
 * Range:     u8 n <= 13, u16 n <= 31, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   factorial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* determine row r: r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;   /* first index of row r */
    idx_t k     = n - start;          /* column index (0‑based) */

    seq_t fact = factorial(r); /* r! */

    if (k == 0) {
        /* T(r,0) = r! * Σ_{j=0}^{r-1} (-1)^j/(j+1) */
        seq_t sum = 0;
        for (idx_t j = 0; j < r; ++j) {
            seq_t term = fact / (j + 1);
            if (j & 1)
                sum -= term;
            else
                sum += term;
        }
        return sum;
    } else {
        /* T(r,k) = r! * (2^k - 1) / k  for k > 0 */
        seq_t two = pow2(k);         /* 2^k */
        seq_t numerator = fact * (two - 1);
        seq_t result = numerator / (seq_t)k; /* exact division */
        return result;
    }
}
