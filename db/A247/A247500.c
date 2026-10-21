/*
 * A247500  Triangle read by rows: T(n, k) = n!*binomial(n + 1, k)/(k + 1)!, 0
 *          <= k <= n.
 * https://oeis.org/A247500
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = list of T(n,k) over all n,k in row-major order where
 *            T(n,k) = n!*binomial(n+1,k)/(k+1)!, 0 <= k <= n (OEIS A247500)
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 78, u64 n <= 210,
 *            u128 n <= 562, big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "../utils/factorial.h"
#include "../utils/binomial.h"

static inline idx_t _row_from_index(idx_t n)
{
    if (n == 0)
        return 0;
    double d = sqrt((double)(8.0 * (double)n + 1.0));
    idx_t r = (idx_t)((d - 1.0) / 2.0);
    /* correct possible rounding errors */
    while (((__uint128_t)(r + 1) * (r + 2)) / 2 <= n)
        ++r;
    while (((__uint128_t)r * (r + 1)) / 2 > n)
        --r;
    return r;
}

seq_t a(idx_t n)
{
    /* determine row r and column c of the element a(n) */
    idx_t r = _row_from_index(n);
    idx_t c = n - (idx_t)(((unsigned __int128)r * (r + 1)) / 2);

    /* T(r,c) = r! * binomial(r+1,c) / (c+1)! */
    seq_t fac_r   = factorial(r);
    seq_t bin     = binomial(r + 1, c);
    seq_t fac_k1  = factorial(c + 1);

    seq_t term = fac_r * bin;
    term /= fac_k1;  /* exact integer division */
    return term;
}
