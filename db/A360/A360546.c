/*
 * A360546  Triangle read by rows: T(n, m) = (n+1-m)*C(2*n+2-m,
 *          m)*C(3*n-3*m+2, n-m+1)/(2*n-m+2).
 * https://oeis.org/A360546
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(n,m) = (n+1-m)*C(2*n+2-m, m)*C(3*n-3*m+2, n-m+1)/(2*n-m+2)
 *            Sequence a(n) is the triangle read by rows, starting with a(0)=1
 * Range:     u8 n <= 5, u16 n <= 14, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/gcd.h"

/* Small integer square root helper (used only for triangular index calculation) */
static inline idx_t floor_sqrt_idx(idx_t x)
{
    idx_t low = 0, high = x;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        __int128 sq = (__int128)mid * mid;
        if (sq == ( __int128)x) return mid;
        if (sq < ( __int128)x)
            low = mid + 1;
        else
            high = mid ? mid - 1 : 0;
    }
    return high;
}

/* Triangular number: tri(r) = r*(r+1)/2 (uses 128‑bit to avoid overflow) */
static inline idx_t tri(idx_t r)
{
    return (idx_t)(((__int128)r * (r + 1)) >> 1);
}

/* Return the nth term of A360546 (non‑negative offset) */
seq_t a(idx_t n)
{
    /* Determine the row r such that tri(r) <= n < tri(r+1) */
    idx_t r;
    /* For large n a direct formula is faster; for small n linear scan is fine */
    if (n < 5000) {
        r = 0;
        while (tri(r + 1) <= n) ++r;
    } else {
        idx_t sq = floor_sqrt_idx((idx_t)((__int128)8 * n + 1));
        r = (sq - 1) / 2;
        /* Correct potential off‑by‑one caused by integer math */
        while (tri(r + 1) <= n) ++r;
        while (tri(r) > n) --r;
    }

    /* Column index within the row */
    idx_t m = n - tri(r);

    /* Pre‑compute binomial arguments */
    idx_t a1 = 2 * r + 2 - m;
    idx_t b1 = m;
    idx_t a2 = 3 * r - 3 * m + 2;
    idx_t b2 = r - m + 1;

    /* Core terms of the formula */
    seq_t term1   = (seq_t)(r + 1 - m);
    seq_t bin1    = binomial(a1, b1);
    seq_t bin2    = binomial(a2, b2);
    seq_t denom   = (seq_t)(2 * r - m + 2);

    /* Reduce by gcd before multiplication to keep numbers smaller */
    seq_t g = gcd(term1, denom);
    if (g != 1) { term1 /= g; denom /= g; }

    g = gcd(bin1, denom);
    if (g != 1) { bin1 /= g; denom /= g; }

    g = gcd(bin2, denom);
    if (g != 1) { bin2 /= g; denom /= g; }

    /* Multiply the remaining parts */
    seq_t result = term1;
    result *= bin1;
    result *= bin2;

    /* Final division by any remaining denominator part */
    if (denom != 1) result /= denom;

    return result;
}
