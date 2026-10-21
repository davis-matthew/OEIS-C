/*
 * A254609  Triangle read by rows: T(n,k) =
 *          A243757(n)/(A243757(k)*A243757(n-k)).
 * https://oeis.org/A254609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 5^{v5(n!)-v5(k!)-v5((n-k)!)} where k = n - r*(r+1)/2 and
 *            r = floor((sqrt(8*n+1)-1)/2). v5(m!) = Sum_{i>=1} floor(m / 5^i)
 * Range:     u8 n <= 7874, u16 n <= 7874, u32 n <= 7874, u64 n <= 7874,
 *            u128 n <= 7874, big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"

static inline idx_t legendre5(idx_t n) {
    idx_t count = 0;
    for (idx_t d = 5; d && n / d > 0; d *= 5)
        count += n / d;
    return count;
}

static inline idx_t triangular(idx_t r) {
    return (unsigned __int128)r * (r + 1) / 2;
}

seq_t a(idx_t n) {
    /* Find r such that triangular(r) <= n < triangular(r+1). */
    idx_t low = 0, high = n + 1;   /* exclusive high */
    while (high - low > 1) {
        idx_t mid = (low + high) >> 1;
        if (triangular(mid) <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = low;
    idx_t k = n - triangular(r);   /* column index in row r */

    /* Compute exponent via Legendre's formula for 5-adic valuation. */
    idx_t exp = legendre5(r) - legendre5(k) - legendre5(r - k);

    return pow_uint(5, exp);
}
