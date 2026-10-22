/*
 * A372713  Number of divisors of 3n; a(n) = tau(3*n) = A000005(3*n).
 * https://oeis.org/A372713
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ(3 · n) (number of divisors of 3*n)
 * Range:     u8 n <= 9970, u16 n <= 9970, u32 n <= 9970, u64 n <= 9970,
 *            u128 n <= 9970, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* only defined for n≥1, guard just in case */

    /* Convert to a 64‑bit integer for safe arithmetic. */
    uint64_t x = (uint64_t)n;

    /* We will factor n.  All intermediate arithmetic is done on
       64‑bit unsigned integers; the divisor count itself is kept in
       seq_t to respect the required type. */
    seq_t tau_n = 1;          /* τ(n) */
    uint64_t k = 0;           /* exponent of prime 3 in n */

    /* Handle the factor 2 separately. */
    if ((x & 1) == 0) {
        uint64_t exp = 0;
        while ((x & 1) == 0) {
            x >>= 1;
            exp++;
        }
        tau_n *= (seq_t)(exp + 1);
    }

    /* Trial division for odd primes.  The loop guard is written
       as p <= x/p to avoid overflow of p*p. */
    for (uint64_t p = 3; p <= x / p; p += 2) {
        if (x % p == 0) {
            uint64_t exp = 0;
            while (x % p == 0) {
                x /= p;
                exp++;
            }
            tau_n *= (seq_t)(exp + 1);
            if (p == 3) k = exp;
        }
    }

    /* If there is still a prime factor > sqrt(original n). */
    if (x > 1) {
        tau_n *= (seq_t)2;          /* prime contributes factor (1+1) */
        if (x == 3) k = 1;
    }

    /* Now compute τ(3·n).  If n is not divisible by 3, add one
       extra factor of 3, otherwise adjust the exponent of 3. */
    seq_t result;
    if (k == 0) {
        result = tau_n * (seq_t)2;
    } else {
        result = tau_n * (seq_t)(k + 2) / (seq_t)(k + 1);
    }

    return result;
}
