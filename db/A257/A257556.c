/*
 * A257556  Triangle, read by rows, T(n,k)= Sum_{i=0..(n-k)/2}
 *          C(2*k,i)*C(n-2*i-1,k-1).
 * https://oeis.org/A257556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle defined by T(n,k) =
 *            Sum_{i=0..(n-k)/2} C(2*k,i) * C(n-2*i-1,k-1) row‑wise. For a
 *            given global index n (offset 1) the sequence value is the
 *            flattening of the above triangle
 * Range:     u8 n <= 39, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* safety for indices <1 */

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 0;
    while ((r * (r + 1) / 2) < n)
        ++r;

    /* Position k within that row (1‑based) */
    idx_t prev = r * (r - 1) / 2;       /* entries before row r */
    idx_t k = n - prev;                 /* 1 ≤ k ≤ r */

    /* Compute the triangle entry T(r,k) */
    seq_t result = 0;
    idx_t limit = (r - k) / 2;          /* (r-k)/2 floor */

    for (idx_t i = 0; i <= limit; ++i) {
        seq_t a1 = binomial(2 * k, i);
        seq_t a2 = binomial(r - 2 * i - 1, k - 1);
        result += a1 * a2;
    }

    return result;
}
