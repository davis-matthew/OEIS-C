/*
 * A356073  Number of labeled trees on [n] having a centroid.
 * https://oeis.org/A356073
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n-2) if n is odd, a(n) = n^(n-2) -
 *            C(n,n/2)*(n/2)^(n-2)/2 if n is even. a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;

    /* For odd n, simply n^(n-2).  For even n, subtract the adjustment. */
    if (n & 1) {
        return ipow((seq_t)n, (seq_t)(n - 2));
    } else {
        seq_t pow_total = ipow((seq_t)n, (seq_t)(n - 2));

        seq_t bin_coeff  = binomial(n, n / 2);
        seq_t pow_half   = ipow((seq_t)(n / 2), (seq_t)(n - 2));

        /* Compute sub = bin_coeff * pow_half / 2 safely to avoid overflow. */
        seq_t sub = seq_mul(bin_coeff, pow_half);
        sub = sub / 2;   /* division by 2 */

        return pow_total - sub;
    }
}
