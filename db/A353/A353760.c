/*
 * A353760  a(n) = 1 if A353749(n) > A353749(sigma(n)), otherwise 0. Here
 *          A353749(n) = phi(n)*A064989(n), and A064989 shifts the prime
 *          factorization one step towards lower primes.
 * https://oeis.org/A353760
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if A353749(n) > A353749(sigma(n)), otherwise 0 where
 *            A353749(n) = phi(n) * A064989(n) and A064989(n) shifts each
 *            prime factor of n to its immediate lower prime (2 → 1, 3 → 2, 5
 *            → 3, 7 → 5, …)
 * Range:     u8 n <= 17, u16 n <= 1224, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 136 terms through n = 9801 (2026-11-01)
 * Helpers:   factor_u64, is_prime, totient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/factor_u64.h"
#include "../utils/is_prime.h"
#include "../utils/totient.h"

/* Compute σ(i) by trial division up to √i. */
static seq_t sigma_of(idx_t n)
{
    seq_t sum = (seq_t)0;
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            sum += (seq_t)d;
            idx_t d2 = n / d;
            if (d2 != d)
                sum += (seq_t)d2;
        }
    }
    return sum;
}

/* Return the largest prime < n   (n > 2)   ; pp(2) = 1, but we never call it for n==2 */
static inline idx_t prev_prime(idx_t n)
{
    idx_t p = n - 1;
    for (;; --p)
    {
        if (is_prime(p)) return p;
    }
}

/* Compute A064989(n) for an index n (fits in idx_t). */
static seq_t _A064989(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;

    uint64_t primes[64];
    uint64_t exps[64];
    size_t cnt = 0;
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i)
    {
        if (primes[i] == 2)
            continue;               /* previous prime of 2 is 1 → no effect */
        uint64_t prev = prev_prime(primes[i]);   /* previous prime of p_i */
        for (uint64_t j = 0; j < exps[i]; ++j)
            result *= (seq_t)prev;
    }
    return result;
}

/* Compute A353749(n) = phi(n) * A064989(n) for an index n. */
static seq_t _A353749(idx_t n)
{
    seq_t phi_n   = totient(n);
    seq_t a064_n  = _A064989(n);
    return phi_n * a064_n;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;

    seq_t val_n = _A353749(n);

    seq_t sigma_n = sigma_of(n);
    /* sigma(n) is small for n up to at least 10 000; cast safely to idx_t. */
    idx_t sigma_idx = (idx_t)sigma_n;

    seq_t val_sigma = _A353749(sigma_idx);

    return (val_n > val_sigma) ? (seq_t)1 : (seq_t)0;
}
