/*
 * A391482  Triangle read by rows: T(n,k) is the number of labeled commutative
 *          magmas with n elements and k idempotents.
 * https://oeis.org/A391482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = floor((sqrt(8n+1)-1)/2), * k = n -
 *            r*(r+1)/2, where T(n,k) = binomial(n,k) * n^(n*(n-1)/2) *
 *            (n-1)^(n-k)
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 27, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A391482: triangle read by rows.
 *   T(n,k) = binomial(n,k) * n^(n*(n-1)/2) * (n-1)^(n-k)
 *   a(n) is the nth element of the triangle flattened row by row.
 *
 *   Compute a(n) given the index n (0‑based).
 */
#ifndef A391482_H
#define A391482_H

#include "../utils/binomial.h"
#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    /* find the row r that contains index n */
    idx_t r = 0;
    idx_t cum = 0;                      /* total elements before row r */
    while (cum + (r + 1) <= n) {
        cum += r + 1;
        r++;
    }
    /* position k within row r */
    idx_t k = n - cum;

    /* compute T(r,k) */
    seq_t bin   = binomial(r, k);

    idx_t exp1 = r * (r - 1) / 2;      /* exponent for n^(n*(n-1)/2) */
    seq_t pow1 = pow_int((seq_t)r, exp1);

    idx_t exp2 = r - k;                /* exponent for (n-1)^(n-k) */
    seq_t pow2 = (exp2 == 0
                  ? (seq_t)1
                  : pow_int((seq_t)(r ? r - 1 : 0), exp2));

    seq_t result = bin;
    result *= pow1;
    result *= pow2;
    return result;
}

#endif /* A391482_H */
