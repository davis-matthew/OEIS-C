/*
 * A375667  The maximum exponent in the prime factorization of the 5-rough
 *          numbers (A007310).
 * https://oeis.org/A375667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = maximum exponent in the prime factorization of the nth
 *            5‑rough number (the 5‑rough numbers start with 1)
 * Range:     u8 n <= 85, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case: first 5‑rough number is 1. */
    if (n == 1) return 0;

    /* Compute the nth 5‑rough number.
       For n even: 6*(n/2)   - 1   (5, 11, 17, …)
       For n odd : 6*((n-1)/2) + 1 (7, 13, 19, …)   */
    seq_t x;
    if (n & 1) {                     /* n odd (>=3) */
        x = 6 * ((seq_t)(n - 1) / 2) + 1;
    } else {                         /* n even (>=2) */
        x = 6 * ((seq_t) n / 2) - 1;
    }

    /* Factor x and find the largest exponent. */
    seq_t max_exp = 0;
    seq_t temp = x;

    /* Trial division up to sqrt(temp). */
    for (idx_t p = 2; (idx_t)p * (idx_t)p <= temp; ++p) {
        seq_t exp = 0;
        while (temp % p == 0) {
            temp /= p;
            ++exp;
        }
        if (exp > max_exp) max_exp = exp;
    }

    /* If a prime factor remains > 1, it contributes exponent 1. */
    if (temp > 1 && max_exp < 1) max_exp = 1;

    return max_exp;
}
