/*
 * A370236  Triangle read by rows: T(n, k) is the number of partitions of
 *          genus 1 and k parts of the n-set (n >= 4, 2 <= k <= n-2).
 * https://oeis.org/A370236
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,k) = (1/6) * binomial(n,2) * binomial(n-2,k) *
 *            binomial(n-2,k-2) The sequence a(n) (n ≥ 4) lists the triangle
 *            T(n,k) read by rows: row n has k = 2 … n-2, and the rows are
 *            concatenated starting with the first row (n = 4). Thus a(4) =
 *            T(4,2), a(5) = T(5,2), a(6) = T(5,3), … This function comput...
 * Range:     u8 n <= 10, u16 n <= 26, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"

/* Integer square root of a 64‑bit value using 128‑bit multiplication. */
static idx_t isqrt_u64(idx_t u)
{
    idx_t lo = 0, hi = (u < 0xFFFFFFFFFFFFFFFFULL ? u : 0xFFFFFFFFFFFFFFFFULL);
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= u)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;
}

seq_t a(idx_t n)
{
    /* Offset: 4.  Behaviour for n < 4 is irrelevant. */
    if (n < 4) return (seq_t)0;

    /* i = position of the term in the concatenated triangle (1‑based). */
    idx_t i = n - 3;

    /* Find minimal x such that x*(x+1)/2 >= i.
       Then r = x + 3 is the row number. */
    idx_t u = (idx_t)(((unsigned __int128)8 * i) + 1);   /* 1 + 8*i */
    idx_t s = isqrt_u64(u);                              /* floor(sqrt(u)) */
    idx_t x = (s - 1) >> 1;                              /* floor((s-1)/2) */
    unsigned __int128 tri = (unsigned __int128)x * (x + 1) / 2;
    if (tri < i) ++x;                                    /* ceil if needed */

    idx_t r   = x + 3;                                   /* row */
    unsigned __int128 prev_tri = (unsigned __int128)(x - 1) * x / 2;
    idx_t pos = i - (idx_t)prev_tri;                     /* 1‑based position in row */
    idx_t k   = pos + 1;                                 /* k = 2 + (pos-1) */

    /* Compute the binomial coefficients and the product. */
    seq_t b1 = binomial(r, 2);                       /* C(r,2) */
    seq_t b2 = binomial(r - 2, k);                   /* C(r-2,k) */
    seq_t b3 = binomial(r - 2, k - 2);               /* C(r-2,k-2) */

    seq_t num = b1 * b2;
    num     *= b3;
    seq_t res = num / (seq_t)6;                      /* divide by 6 */

    return res;
}
