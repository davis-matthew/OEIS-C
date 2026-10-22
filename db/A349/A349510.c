/*
 * A349510  a(n) = binomial(n^3-floor(((n-1)^3+1)/2), 3*n^2-3*n+1) +
 *          binomial(n^3-floor(((n-1)^3+2)/2), 3*n^2-3*n+1).
 * https://oeis.org/A349510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n^3 - floor(((n-1)^3+1)/2), 3*n^2-3*n+1) +
 *            binomial(n^3 - floor(((n-1)^3+2)/2), 3*n^2-3*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* n = 0 is a special case; (n-1) would under‑flow with unsigned types. */
    if (n == 0)
        return (seq_t)0;

    /* Compute n^3 and (n-1)^3.  All intermediates fit in uint64_t for the
       required range (up to n ≈ 10 000). */
    idx_t n3   = n * n * n;          /* n^3      */
    idx_t nm1  = n - 1;              /* n-1      */
    idx_t nm1_3 = nm1 * nm1 * nm1;   /* (n-1)^3  */

    /* k = 3*n^2 - 3*n + 1  */
    idx_t k = 3 * n * n - 3 * n + (idx_t)1;

    /* First binomial upper argument:  n^3 - floor(((n-1)^3+1)/2) */
    idx_t a1 = n3 - ((nm1_3 + (idx_t)1) / (idx_t)2);

    /* Second binomial upper argument: n^3 - floor(((n-1)^3+2)/2) */
    idx_t a2 = n3 - ((nm1_3 + (idx_t)2) / (idx_t)2);

    /* Compute the two binomial coefficients.  The helper returns seq_t. */
    seq_t bin1 = binomial(a1, k);
    seq_t bin2 = binomial(a2, k);

    /* Return their sum. */
    return bin1 + bin2;
}
