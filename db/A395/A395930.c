/*
 * A395930  Triangle read by rows: T(n, k) = 4^n * [x^k] hypergeom([-n, -n,
 *          1/2], [1], x).
 * https://oeis.org/A395930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r, k) with r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2; T(r,k) = 4^(r-k) * C(r,k)^2 * C(2*k,k) * k!
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial, factorial, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <math.h>
#include "../utils/pow_uint.h"      /* seq_t pow_uint(uint64_t base, idx_t exp); */
#include "../utils/binomial.h"     /* seq_t binomial(idx_t n, idx_t k); */
#include "../utils/factorial.h"    /* seq_t factorial(idx_t n); */

static seq_t compute_T(idx_t n, idx_t k)
{
    /* 4^(n-k) */
    seq_t res = pow_uint(4ULL, n - k);
    /* square binomial(n,k) */
    seq_t bin = binomial(n, k);
    res *= bin;
    res *= bin;
    /* multiply by binomial(2k,k) */
    res *= binomial(2 * k, k);
    /* multiply by k! */
    res *= factorial(k);
    return res;
}

seq_t a(idx_t n)
{
    /* determine row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1.0) / 2.0);
    idx_t start = r * (r + 1) / 2;
    idx_t k = n - start;
    return compute_T(r, k);
}
