/*
 * A269317  Ternary version of A257341: take the proper fractions in lowest
 *          terms in ascending order by denominators and within each
 *          denominator in ascending order by numerators. a(n) is the n-th
 *          ternary digit of the n-th fraction.
 * https://oeis.org/A269317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   a(n) is the n-th ternary digit of the n-th proper fraction in
 *            lowest terms, ordered by denominator then numerator
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd_u64.h"

/* small GCD for 64‑bit unsigned integers */

/* pre‑compute Euler’s totient φ up to a bound sufficient for n≤10000 */
#define MAX_D 5000
static uint32_t phi[MAX_D + 1];
static bool phi_init = false;

/* simple sieve for φ */
static void init_phi(void) {
    if (phi_init) return;
    for (uint32_t i = 0; i <= MAX_D; ++i) phi[i] = i;
    phi[0] = 0;
    phi[1] = 1;
    for (uint32_t i = 2; i <= MAX_D; ++i) {
        if (phi[i] == i) { /* i is prime */
            for (uint32_t j = i; j <= MAX_D; j += i) {
                phi[j] -= phi[j] / i;
            }
        }
    }
    phi_init = true;
}

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, but guard */

    init_phi();

    /* find denominator d such that sum_{k=2}^{d-1} φ(k) < n ≤ sum_{k=2}^{d} φ(k) */
    idx_t cum = 0;
    idx_t d = 2;
    while (d <= MAX_D && cum + phi[d] < n) {
        cum += phi[d];
        ++d;
    }
    /* Assuming n ≤ 10000, d will be found before MAX_D */
    idx_t r = n - cum; /* rank within φ(d), 1‑based */

    /* find the r‑th coprime numerator i (1 ≤ i < d) */
    idx_t count = 0;
    idx_t i;
    for (i = 1; i < d; ++i) {
        if (gcd_u64(i, d) == 1) {
            ++count;
            if (count == r) break;
        }
    }

    /* compute the n‑th ternary digit of the fraction i/d */
    seq_t num = (seq_t)i;
    seq_t den = (seq_t)d;
    seq_t digit = 0;

    for (idx_t pos = 1; pos <= n; ++pos) {
        num = num * 3;
        digit = num / den;
        num = num % den;
        if (pos == n) break;
    }

    return digit; /* 0,1 or 2 */
}
