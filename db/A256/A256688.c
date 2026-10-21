/*
 * A256688  From third root of Riemann zeta function: form Dirichlet series
 *          Sum b(n)/n^x whose cube is zeta function; sequence gives numerator
 *          of b(n).
 * https://oeis.org/A256688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, mult
 * Formula:   a(n) is multiplicative. For a prime power p^e, let b(p^e) =
 *            binom(1/3 + e - 1, e) = ∏_{t=1..e} (3t-2) / (3t). The numerator
 *            of b(p^e), after canceling the denominator, is the value that
 *            contributes multiplicatively to a(n). Thus a(n) = ∏_{p^e‖n}
 *            numerator(b(p^e))
 * Range:     u8 n <= 31, u16 n <= 511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/gcd.h"

static idx_t *spf = NULL;          /* smallest prime factor table */
static idx_t spf_cap = 0;          /* current capacity of spf */

/* Build smallest prime factor table up to n (inclusive) */
static void ensure_spf(idx_t n) {
    if (n <= spf_cap) return;
    free(spf);
    spf = (idx_t*)malloc((n + 1) * sizeof(idx_t));
    if (!spf) return;                 /* allocation failed, abort silently */

    for (idx_t i = 0; i <= n; ++i) spf[i] = 0;
    for (idx_t i = 2; i <= n; ++i) {
        if (spf[i] == 0) {
            spf[i] = i;
            if (i <= n / i) {
                for (idx_t j = i * i; j <= n; j += i)
                    if (spf[j] == 0) spf[j] = i;
            }
        }
    }
    spf[1] = 1;
    spf_cap = n;
}

/* Numerator of b(p^e) after canceling the denominator (3^e * e!)   */
static seq_t pp_num(idx_t e) {
    if (e == 0) return (seq_t)1;
    seq_t num = (seq_t)1;
    seq_t denom = (seq_t)1;
    for (idx_t t = 1; t <= e; ++t) {
        seq_t mult_num = (seq_t)(3 * t - 2);
        seq_t mult_den = (seq_t)(3 * t);
        num *= mult_num;
        denom *= mult_den;
        seq_t g = gcd(num, denom);
        if (g != (seq_t)1) {
            num /= g;
            denom /= g;
        }
    }
    return num;                       /* denom is 1 after full reduction, but omit it */
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;      /* out of range */
    ensure_spf(n);
    seq_t result = (seq_t)1;
    idx_t x = n;
    while (x > 1) {
        idx_t p = spf[x];
        idx_t e = 0;
        while (x % p == 0) {
            x /= p;
            ++e;
        }
        seq_t part = pp_num(e);
        result *= part;
    }
    return result;
}
