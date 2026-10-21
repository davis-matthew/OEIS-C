/*
 * A283616  a(n) = Product_{k=2..floor(sqrt(2n-1)/2)+1} (2n-1) mod (2k-1).
 * https://oeis.org/A283616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=2..floor(sqrt(2n-1)/2)+1} (2n-1) mod (2k-1)
 * Range:     u8 n <= 44, u16 n <= 131, u32 n <= 251, u64 n <= 804,
 *            u128 n <= 2274, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>

/* Integer square root for unsigned 64‑bit values.
 * Returns floor(sqrt(x)).  Uses long double sqrt for speed
 * and corrects by at most one step. */
static inline idx_t u64_isqrt(idx_t x) {
    if (x < 2) return x;
    long double sq = sqrt((long double)x);
    idx_t r = (idx_t)sq;
    /* Correct possible off‑by‑one errors due to rounding. */
    while ((unsigned __int128)(r + 1) * (r + 1) <= x) r++;
    while ((unsigned __int128)r * r > x) r--;
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset is 1, undefined for n<1 */
    /* m = 2n – 1  (odd integer) */
    idx_t m = 2 * n - 1;

    /* Determine the maximum k:  kmax = floor(sqrt(m)/2) + 1 */
    idx_t sqrt_m = u64_isqrt(m);
    idx_t kmax = sqrt_m / 2 + 1;

    /* For n = 1 or 2, kmax < 2 → empty product → 1. */
    if (kmax < 2) return (seq_t)1;

    seq_t prod = 1;
    for (idx_t k = 2; k <= kmax; ++k) {
        idx_t d = 2 * k - 1;          /* odd modulus */
        seq_t r = (seq_t)m % (seq_t)d;/* remainder */
        prod *= r;
        if (prod == 0) break;          /* early exit if zero */
    }
    return prod;
}
