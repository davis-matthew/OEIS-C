/*
 * A303987  Triangle read by rows: T(n, k) = (binomial(n,k)*binomial(n+k,k))^2
 *          = A063007(n, k)^2, for n >= 0, k = 0..n.
 * https://oeis.org/A303987
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (binomial(r, k) * binomial(r + k, k))^2, where r =
 *            floor((sqrt(8*n + 1) - 1)/2), k = n - r*(r + 1)/2
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 40, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

/* Integer square root for 128‑bit values */
static inline idx_t isqrt128(unsigned __int128 v) {
    idx_t lo = 0, hi = (idx_t)-1;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;           /* floor sqrt */
}

seq_t a(idx_t n) {
    /* compute row index r */
    unsigned __int128 target = (unsigned __int128)n * 8 + 1;
    idx_t sqrt_val = isqrt128(target);
    idx_t r = (sqrt_val - 1) >> 1;

    /* position k within the row */
    idx_t cum = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - cum;

    /* binomial values */
    seq_t t1 = binomial(r, k);
    seq_t t2 = binomial(r + k, k);

    /* square the product */
    seq_t prod = t1 * t2;
    return prod * prod;
}
