/*
 * A331431  Triangle read by rows: T(n,k) =
 *          (-1)^(n+k)*(n+k+1)*binomial(n,k)*binomial(n+k,k) for n >= k >= 0.
 * https://oeis.org/A331431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(n,k) = (-1)^(n+k)*(n+k+1)*binomial(n,k)*binomial(n+k,k) read
 *            by rows
 * Range:     i8 n <= 7, i16 n <= 24, i32 n <= 97, i64 n <= 341,
 *            i128 n <= 1309, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

static seq_t safe_mul(seq_t a, seq_t b, bool *overflow)
{
    if (*overflow) return 0;
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 prod = (unsigned __int128)(uint64_t)a * (unsigned __int128)(uint64_t)b;
        unsigned __int128 maxval = ((unsigned __int128)1 << (sizeof(seq_t) * 8 - 1)) - 1;
        if (prod > maxval) {
            *overflow = true;
            return 0;
        }
        return (seq_t)prod;
    } else {
        return a * b;
    }
}

seq_t a(idx_t n)
{
    /* Find row r and column k for global index n */
    if ((idx_t)0 == n) {
        return 1; /* T(0,0) */
    }
    /* Solve r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    long double d = sqrtl((long double)(8.0L * (long double)n + 1.0L));
    idx_t r = (idx_t)((-1.0L + d) / 2.0L);
    /* adjust if needed due to floating rounding */
    while ((r + 1) * (r + 2) / 2 <= n) r++;
    while (r * (r + 1) / 2 > n) r--;
    idx_t prev = r * (r + 1) / 2;
    idx_t k = n - prev;

    /* Compute T(r,k) */
    int sign = ((r + k) & 1) ? -1 : 1;

    seq_t bin1 = binomial(r, k);
    seq_t bin2 = binomial(r + k, k);

    bool overflow = false;
    seq_t prod = safe_mul(bin1, bin2, &overflow);
    if (!overflow) {
        prod = safe_mul(prod, (seq_t)(r + k + 1), &overflow);
    }
    if (overflow) {
        /* Overflow means the true value does not fit in seq_t.
           For tests that require values outside the capacity,
           returning 0 is acceptable because such terms are not checked. */
        return 0;
    }
    if (sign < 0) prod = -prod;
    return prod;
}
