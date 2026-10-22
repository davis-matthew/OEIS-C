/*
 * A316594  a(n) equals the coefficient of x^n in Sum_{m>=0} (x^m + 4 +
 *          1/x^m)^m for n >= 1.
 * https://oeis.org/A316594
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) equals the coefficient of x^n in ∑_{m≥0} (x^m + 4 + x⁻ᵐ)^m
 *            * The coefficient is obtained by summing, for each divisor m of
 *            n with m≥n/m, * all combinatorial terms where the exponent
 *            balance condition holds
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Helpers:   binom, power4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binom.h"   /* binom(idx_t a, idx_t b) */
#include "../utils/power4.h"

/* add the contributions of a single divisor m of n (with m ≥ d = n/m) */
static void add_divisor_contrib(idx_t m, idx_t d, seq_t *res) {
    /* k ranges from 0 to ⌊(m-d)/2⌋ */
    idx_t max_k = (m - d) / 2;
    for (idx_t k = 0; k <= max_k; ++k) {
        idx_t i = k + d;                 /* number of x^m factors      */
        idx_t j = m - 2 * k - d;         /* number of 4  factors       */
        /* number of ways to pick i positions for x^m from m slots */
        seq_t term = binom(m, i);
        /* then pick k positions for x⁻ᵐ from the remaining m‑i slots */
        term *= binom(m - i, k);
        /* multiply by 4^j */
        term *= power4(j);
        *res += term;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;               /* safety for offset 0 */

    seq_t result = 0;

    /* iterate over all divisors i of n up to √n */
    for (idx_t i = 1; i <= n / i; ++i) {
        if (n % i == 0) {
            idx_t d = i;           /* smaller divisor      */
            idx_t m = n / i;       /* larger divisor       */
            /* only process the larger divisor (m ≥ d) */
            if (m >= d) {
                add_divisor_contrib(m, d, &result);
            }
            /* the smaller divisor gives m' = d, d' = m which is smaller than m';
               it cannot satisfy m' ≥ d', so no contribution is needed. */
        }
    }

    return result;
}
