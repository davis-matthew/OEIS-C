/*
 * A349727  Triangle read by rows, T(n, k) = [x^(n - k)] hypergeom([-n, -1 +
 *          n], [-n], x).
 * https://oeis.org/A349727
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(2r - k - 2, r - k) where r = ⌊(√(8n+1)-1)/2⌋ and
 *            k = n - r(r+1)/2. This equals the coefficient of x^(r-k) in
 *            (1-x)^(r-1)
 * Range:     u8 n <= 20, u16 n <= 44, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case for n = 0; the triangle starts with 1. */
    if (n == 0)
        return (seq_t)1;

    /* Compute r as the largest integer with r(r+1)/2 ≤ n. */
    idx_t r = (idx_t)((sqrt((double)8.0 * (double)n + 1.0) - 1.0) / 2.0);

    /* Correct possible rounding errors. */
    while (r * (r + 1) / 2 > n)
        --r;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Offset within the r‑th row. */
    idx_t k = n - r * (r + 1) / 2;

    /* The last element of each row is always 1. */
    if (k == r)
        return (seq_t)1;

    /* Compute the binomial coefficient (2r - k - 2 choose r - k). */
    idx_t a_val = 2 * r - k - 2;
    idx_t b_val = r - k;
    return binomial(a_val, b_val);
}
