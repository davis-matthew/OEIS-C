/*
 * A353766  a(n) = 1 if A353749(n) divides A353749(sigma(n)), and 0 otherwise.
 *          Here A353749(k) = phi(k) * A064989(k), and A064989 shifts the
 *          prime factorization one step towards lower primes.
 * https://oeis.org/A353766
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 17, u16 n <= 9001, u32 n <= 9001, u64 n <= 9001,
 *            u128 n <= 9001, big any n
 * Helpers:   factor_u64, is_prime_small, phi, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1 if A353749(n) divides A353749(sigma(n)), else 0.
 * (A353749(n) = phi(n) * A064989(n); A064989 shifts prime factorisation
 * towards the previous prime.)
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#include "../utils/phi.h"           /* seq_t phi(idx_t m);          */
#include "../utils/factor_u64.h"    /* void factor_u64(uint64_t n, uint64_t *pr, uint64_t *ex, size_t *cnt); */
#include "../utils/sigma_of.h"     /* seq_t sigma_of(idx_t n);     */
#include "../utils/is_prime_small.h"/* bool is_prime_small(seq_t n);*/

/* Multiply a prime power q^e into result, using seq_t arithmetic. */
static seq_t pow_seq(seq_t q, uint64_t e)
{
    seq_t r = 1;
    while (e--)
        r *= q;
    return r;
}

/* A064989(n): shift each prime factor of n to the previous prime
 * (or 1 if no previous prime). For example 12 = 2²·3 → (1²·2).
 * The result is always ≥1. */
static seq_t shift_product(idx_t n)
{
    if (n <= 1)
        return 1;

    /* For n ≤ 10000 the number of prime factors is at most 4. */
    uint64_t pr[8];
    uint64_t ex[8];
    size_t cnt = 0;

    factor_u64((uint64_t)n, pr, ex, &cnt);

    seq_t result = 1;
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p = pr[i];
        uint64_t e = ex[i];

        uint64_t q;
        if (p <= 2) {
            q = 1;               /* shift 2 → 1 */
        } else {
            uint64_t cand = p - 1;
            while (cand >= 2 && !is_prime_small((seq_t)cand))
                --cand;
            /* cand is guaranteed ≥ 2 because 2 is prime. */
            q = cand;
        }

        result *= pow_seq((seq_t)q, e);
    }
    return result;
}

/* A353749(k) = phi(k) * A064989(k). */
static seq_t A353749_of(idx_t k)
{
    seq_t phi_val = phi(k);
    seq_t shift_val = shift_product(k);
    return phi_val * shift_val;
}

seq_t a(idx_t n)
{
    seq_t val_n = A353749_of(n);
    seq_t sigma_val = sigma_of(n);
    idx_t sigma_idx = (idx_t)sigma_val;      /* sigma(n) < 10⁵ for n ≤ 10⁴ */
    seq_t val_sigma = A353749_of(sigma_idx);
    return (val_sigma % val_n == 0) ? 1 : 0;
}
