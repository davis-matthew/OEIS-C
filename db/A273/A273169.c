/*
 * A273169  Numerators of coefficient triangle for integrated even powers of
 *          cos(x) in terms of x and sin(2*m*x).
 * https://oeis.org/A273169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, frac, easy
 * Formula:   a(n) = numerator of the rational R(r,c) where (r,c) is the *
 *            row/column of the n-th entry read row‑by‑row from the triangle *
 *            defined by * R(r,0) = binom(2r,r) / 4^r * R(r,c) = binom(2r,r‑c)
 *            / (c * 4^r) (c > 0). * The index n is converted to (r,c) by the
 *            triangular indexing * rule: S(r) = r(r+1)/...
 * Range:     u8 n <= 9, u16 n <= 31, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial, gcd, mul_small, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/mul_small.h"
#include "../utils/gcd.h"

/* Integer square root: floor(sqrt(x)).  Uses binary search with
 * 128‑bit arithmetic to avoid overflow. */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low = 0, high = ((idx_t)1 << 32);   /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        idx_t mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2 */
    idx_t r = (isqrt_u64(1 + 8 * n) - 1) / 2;
    while (((unsigned __int128)(r + 1) * (r + 2)) / 2 <= n) r++;
    while (((unsigned __int128)r * (r + 1)) / 2 > n) r--;

    idx_t start = (unsigned __int128)r * (r + 1) / 2;
    idx_t c = n - start;   /* column */

    seq_t bin;
    seq_t denom;

    if (c == 0) {
        bin = binomial(2 * r, r);
        denom = pow2(2 * r);          /* 4^r = 2^(2r) */
    } else {
        bin = binomial(2 * r, r - c);
        seq_t pow = pow2(2 * r);
        denom = mul_small(pow, (unsigned int)c);  /* c * 4^r */
    }

    seq_t g = gcd(bin, denom);
    seq_t num = bin / g;   /* numerator after reduction */

    return num;
}
