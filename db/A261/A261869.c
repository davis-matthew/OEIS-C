/*
 * A261869  First differences of A055615.
 * https://oeis.org/A261869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (n+1)*mu(n+1) - n*mu(n)
 * Range:     i8 n <= 64, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Validated: standard, pass, 79 terms through n = 9934 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Möbius function for the divisor in the loop */
static inline int mu_of(idx_t d) {
    int n = (int)d;
    int sign = 1;
    int i = 2;
    while ((int64_t)i * i <= n) {
        if (n % i == 0) {
            int cnt = 0;
            while (n % i == 0) {
                n /= i;
                ++cnt;
                if (cnt > 1) return 0;        /* square factor */
            }
            sign = -sign;
        }
        ++i;
    }
    if (n > 1) sign = -sign;
    return sign;
}

seq_t a(idx_t n)
{
    int mu_n   = mu_of(n);      /* μ(n)   */
    int mu_np1 = mu_of(n + 1);  /* μ(n+1) */

    return ((seq_t)(n + 1) * (seq_t)mu_np1) - ((seq_t)n * (seq_t)mu_n);
}
