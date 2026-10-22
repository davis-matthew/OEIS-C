/*
 * A389629  Expansion of (1/x) * Series_Reversion( x * (1 - x)^2 * (1 - x^2 /
 *          (1 - x)^3) ).
 * https://oeis.org/A389629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(n+1)) * Σ_{k=0..⌊n/2⌋} binomial(n+k,k) *
 *            binomial(3n+k+1, n-2k)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 43,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

static __uint128_t binom128(idx_t n, idx_t k) {
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    __uint128_t r = 1;
    for (idx_t i = 1; i <= k; ++i)
        r = r * (n - k + i) / i;
    return r;
}

seq_t a(idx_t n) {
    idx_t maxk = n / 2U;
    /* Path for 64‑bit or smaller seq_t: use 128‑bit intermediates to avoid overflow. */
    if (sizeof(seq_t) <= sizeof(uint64_t)) {
        __uint128_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            __uint128_t b1 = binom128(n + k, k);
            __uint128_t b2 = binom128(3 * n + k + 1, n - 2 * k);
            sum += b1 * b2;
        }
        sum /= (n + 1);
        return (seq_t)sum;
    }
    /* For larger seq_t (arbitrary‑precision), rely on the utils binom routine. */
    else {
        seq_t sum = 0;
        for (idx_t k = 0; k <= maxk; ++k) {
            seq_t b1 = binom(n + k, k);
            seq_t b2 = binom(3 * n + k + 1, n - 2 * k);
            sum += b1 * b2;
        }
        return sum / ((seq_t)n + 1);
    }
}
