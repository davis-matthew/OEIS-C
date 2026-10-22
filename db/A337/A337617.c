/*
 * A337617  T(n, k) = (n + 1)*2^(n + k)*hypergeom([-n, k - n + 1], [2], 1/2),
 *          triangle read by rows for 0 <= k <= n.
 * https://oeis.org/A337617
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 27, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A337617:  T(n, k) = (n + 1)*2^(n + k)*hypergeom([-n, k - n + 1], [2], 1/2),
   triangle read by rows for 0 <= k <= n.
   The function a(idx_t n) returns the n‑th term of this triangle
   (a(0)=1, a(1)=4, a(2)=6, a(3)=18, …). */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find row r and column k such that n = r*(r+1)/2 + k.  */
    idx_t r = 0;
    /* Use a loop (r is at most about sqrt(2*n)) */
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;

    /* Special case k == r  */
    if (k == r) {
        seq_t pow_n   = pow2(r);          /* 2^n   */
        seq_t pow_n1  = pow2(r + 1);      /* 2^(n+1) */
        return pow_n * (pow_n1 - 1);
    }

    /* General case 0 <= k < r   */
    seq_t sum_pos = 0;   /* add terms with even j  */
    seq_t sum_neg = 0;   /* add terms with odd  j  */

    idx_t max_j = r - k;
    for (idx_t j = 0; j <= max_j; ++j) {
        /* Term = 2^(n-k-j) * C(n+1, j) * C(2n-j-k, n)  */
        seq_t term = pow2(r - k - j);                         /* 2^(n-k-j) */
        term *= binomial(r + 1, j);                           /* C(n+1, j) */
        term *= binomial(2 * r - j - k, r);                   /* C(2n-j-k, n) */

        if ((j & 1) == 0)
            sum_pos += term;
        else
            sum_neg += term;
    }

    /* The alternating sum: sum_pos - sum_neg  (positive). */
    seq_t diff = sum_pos - sum_neg;

    /* Final multiplier 2^(2k+1) */
    seq_t mult = pow2(2 * k + 1);
    return mult * diff;
}
