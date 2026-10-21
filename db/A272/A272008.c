/*
 * A272008  a(n) is the numerator of the fractional part of sigma(n)/n, where
 *          sigma(n) is the sum of the divisors of n.
 * https://oeis.org/A272008
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = (sigma(n) mod n) / gcd(sigma(n) mod n, n)
 * Range:     u8 n <= 183, u16 n <= 9628, u32 n <= 9628, u64 n <= 9628,
 *            u128 n <= 9628, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where sigma(n) is the sum of all positive divisors of n */

#include "../utils/gcd.h"

static inline idx_t isqrt_u64(idx_t n) {
    idx_t low = 0, high = n, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        if (mid == 0) { ans = 0; break; }
        if (mid <= n / mid) { ans = mid; low = mid + 1; }
        else { high = mid - 1; }
    }
    return ans;
}

seq_t a(idx_t n) {
    /* Sum of divisors sigma(n) */
    seq_t sigma = 0;
    idx_t limit = isqrt_u64(n);
    for (idx_t i = 1; i <= limit; ++i) {
        if (n % i == 0) {
            sigma += (seq_t)i;
            idx_t j = n / i;
            if (j != i) sigma += (seq_t)j;
        }
    }

    seq_t r = sigma % (seq_t)n;       /* remainder of sigma(n) modulo n */
    if (r == 0) return 0;             /* perfect or multiply‑perfect numbers */

    seq_t g = gcd(r, (seq_t)n);       /* reduce the fraction (r / n) */
    return r / g;
}
