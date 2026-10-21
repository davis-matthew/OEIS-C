/*
 * A292288  Numerators of smallest denominator of a proper fraction that has a
 *          nontrivial anomalous cancellation in base b.
 * https://oeis.org/A292288
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac, base
 * Formula:   a(n) = value of the numerator of the fraction with * the
 *            smallest denominator that exhibits a non‑trivial anomalous *
 *            cancellation in base n. The function searches for the minimal *
 *            denominator d and returns the corresponding numerator n. * * The
 *            algorithm: * - If the base n is prime, a(n) = n +...
 * Range:     u8 n <= 110, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   gcd64, is_prime_uint, remove_digit, to_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd64.h"
#include "../utils/is_prime_uint.h"
#include "../utils/remove_digit.h"
#include "../utils/to_base.h"

/* Greatest common divisor for 64‑bit unsigned integers */

seq_t a(idx_t n)
{
    uint64_t base = (uint64_t)n;
    if (base <= 1) return (seq_t)0;   /* undefined for bases < 2 */

    /* Prime bases have the closed form a(n) = n + 1 */
    if (is_prime_uint(base)) return (seq_t)(base + 1);

    /* Search for the minimal denominator */
    const int MAX_DIGITS = 64;          /* enough for bases up to 10^4 */
    uint32_t d_digits[MAX_DIGITS];
    int d_len;
    uint32_t n_digits[MAX_DIGITS];
    int n_len;

    for (uint64_t d = 2;; ++d) {
        to_base(d, (uint32_t)base, d_digits, &d_len);

        /* Iterate over all non‑zero digits in the denominator */
        for (int pos = 0; pos < d_len; ++pos) {
            uint32_t k = d_digits[pos];
            if (k == 0) continue;

            uint64_t d_prime = remove_digit(d_digits, d_len, pos, (uint32_t)base);
            if (d_prime == 0) continue;   /* leading zero would give zero denom */

            uint64_t g = gcd64(d, d_prime);
            if (g <= 1) continue;        /* no suitable n' exists */

            uint64_t d1 = d / g;          /* d  = g * d1 */
            uint64_t d2 = d_prime / g;    /* d' = g * d2  ->  d2 = d'/g */

            /* n' must be a multiple of d2;  n' = m * d2, 1 <= m < g */
            for (uint64_t m = 1; m < g; ++m) {
                uint64_t n = m * d1;      /* n = m * (d/g) */

                if (n >= d) continue;     /* proper fraction required */

                to_base(n, (uint32_t)base, n_digits, &n_len);

                /* Find a position of digit k in the numerator */
                for (int pos_n = 0; pos_n < n_len; ++pos_n) {
                    if (n_digits[pos_n] != k) continue;

                    uint64_t n_prime_candidate =
                        remove_digit(n_digits, n_len, pos_n, (uint32_t)base);
                    uint64_t n_prime = m * d2;   /* expected n' */

                    if (n_prime_candidate == n_prime) {
                        return (seq_t)n;
                    }
                }
            }
        }
    }
    /* Should never reach here */
    return (seq_t)0;
}
