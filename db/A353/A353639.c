/*
 * A353639  a(n) = 1 if n is odd and A064989(sigma(n)) > A064989(n), otherwise
 *          0. Here A064989 shifts the prime factorization one step towards
 *          lower primes, and sigma is the sum of divisors function.
 * https://oeis.org/A353639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n is odd and A064989(σ(n)) > A064989(n), otherwise
 *            0. (A064989 shifts each prime factor p^e to the previous prime
 *            (or 1 if p=2) and multiplies the resulting powers.)
 * Range:     u8 n <= 7920, u16 n <= 8959, u32 n <= 8959, u64 n <= 8959,
 *            u128 n <= 8959, big any n
 * Helpers:   _smallest_prime_factor, factor_u64, is_prime, pow_seq, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/sigma_of.h"
#include "../utils/pow_seq.h"
#include "../utils/_smallest_prime_factor.h"

/* Compute A064989 for a u64 value. */
static seq_t A064989(uint64_t x)
{
    if (x <= 1)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t n = x;

    while (n > 1) {
        uint64_t p = _smallest_prime_factor(n);
        uint64_t e = 0;
        while (n % p == 0) {
            n /= p;
            ++e;
        }

        uint64_t q;
        if (p == 2) {
            q = 1;
        } else {
            /* Find the previous prime ≤ p‑1. */
            uint64_t cand = p - 1;
            while (cand > 1 && !is_prime((idx_t)cand))
                --cand;
            q = (cand > 1) ? cand : 1;
        }

        seq_t power = pow_seq((seq_t)q, (int)e);
        result *= power;
    }
    return result;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;
    if ((n & 1u) == 0)   /* even n */
        return (seq_t)0;

    /* Shift n and σ(n) via A064989. */
    seq_t shift_n = A064989((uint64_t)n);

    seq_t sigma_n = sigma_of(n);           /* sum of divisors of n */
    uint64_t sigma_val = (uint64_t)sigma_n; /* n, σ(n) fit in 64‑bit here */

    seq_t shift_sigma = A064989(sigma_val);

    return (shift_sigma > shift_n) ? (seq_t)1 : (seq_t)0;
}
