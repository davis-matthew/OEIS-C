/*
 * A322655  Numerator of (Sum_{d|n} sigma(d)) / sigma(n).
 * https://oeis.org/A322655
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( (Sum_{d|n} sigma(d)) / sigma(n) ) a(n) =
 *            A007429(n) / gcd( A000203(n), A007429(n) ) where A007429(n) =
 *            Sum_{d|n} sigma(d) and A000203(n) = sigma(n)
 * Range:     u8 n <= 47, u16 n <= 8447, u32 n <= 9837, u64 n <= 9837,
 *            u128 n <= 9837, big any n
 * Helpers:   gcd, ipow_idx, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation computes A007429(n) by multiplicativity:
   For n = ∏ p_i^e_i,
       Sum_{d|n} sigma(d) = ∏ ( Σ_{j=0}^{e_i} sigma(p_i^j) ).
   We factor n by trial division (fast enough for n <= 10 000) and use
   the provided sigma_of and gcd utilities. */

#include <stdint.h>
#include "../utils/sigma_of.h"
#include "../utils/gcd.h"
#include "../utils/ipow_idx.h"

/* Helper: computes Σ_{j=0}^{e} sigma(p^j) for a prime power. */
static seq_t sum_sigma_prime_pow(idx_t p, idx_t e) {
    seq_t sum = 0;
    for (idx_t j = 0; j <= e; ++j) {
        idx_t val = ipow_idx(p, j); /* p^j */
        sum += sigma_of(val);
    }
    return sum;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* Undefined for n < 1; guard against accidental call */

    /* Compute sigma(n) */
    seq_t sigma_n = sigma_of(n);

    /* Compute Sum_{d|n} sigma(d) by multiplicativity */
    seq_t sum_sigma = 1;

    idx_t temp = n;
    for (idx_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            idx_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                ++exp;
            }
            sum_sigma *= sum_sigma_prime_pow(p, exp);
        }
    }
    if (temp > 1) { /* Remaining prime factor */
        sum_sigma *= sum_sigma_prime_pow(temp, 1);
    }

    /* Reduce fraction to its numerator */
    seq_t g = gcd(sum_sigma, sigma_n);
    return sum_sigma / g;
}
