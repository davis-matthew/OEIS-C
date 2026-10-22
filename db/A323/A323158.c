/*
 * A323158  If n is a square, a(n) = 1-(n mod 2), otherwise a(n) = (n mod 2);
 *          a(n) = A049820(n) mod 2, where A049820(n) = n - number of divisors
 *          of n.
 * https://oeis.org/A323158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9538, u16 n <= 9538, u32 n <= 9538, u64 n <= 9538,
 *            u128 n <= 9538, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS: A323158
 * Formula: If n is a perfect square, a(n) = 1 - (n mod 2); otherwise a(n) = (n mod 2).
 *
 * The function below implements this definition without external helpers,
 * ensuring it compiles with both C11 and C++17 environments.
 */
#include <stdint.h>

/* integer square‑root for 64‑bit unsigned values, implemented with binary search */
static inline idx_t isqrt_u64(idx_t n) {
    idx_t lo = 0, hi = n >> 1U;
    if (n <= 1U) return n;
    hi++;                       /* hi is exclusive; start with n/2 + 1 for n>1 */
    while (lo < hi) {
        idx_t mid = lo + ((hi - lo) >> 1U);
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq <= n) {
            lo = mid + 1U;
        } else {
            hi = mid;
        }
    }
    return lo - 1U;
}

seq_t a(idx_t n) {
    /* n mod 2 as a seq_t value */
    seq_t mod2 = (seq_t)(n & 1U);

    /* check if n is a perfect square */
    idx_t r = isqrt_u64(n);
    if ((unsigned __int128)r * (unsigned __int128)r == n) {
        /* square: 1 - (n mod 2) */
        return (seq_t)1U - mod2;
    } else {
        /* not a square: n mod 2 */
        return mod2;
    }
}
