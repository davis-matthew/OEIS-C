/*
 * A356637  a(n) = A000265(A263931(n)).
 * https://oeis.org/A356637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏(p odd prime ≤ n) p^exp_p, exp_p = * Σ_{k≥1}
 *            floor(2n/p^k) – 2·floor(n/p^k) (Legendre's formula)
 * Range:     u8 n <= 13, u16 n <= 28, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Static prime table */
static idx_t *prime_list = NULL;   /* array of found primes   */
static size_t prime_count = 0;     /* number of primes stored */
static idx_t prime_limit = 0;      /* largest integer up to which primes are known */

/* Extend the prime table up to 'limit' (inclusive) */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    /* Make sure the prime 2 is present if needed */
    if (limit >= 2 && prime_count == 0) {
        prime_list = (idx_t*)realloc(prime_list, sizeof(idx_t));
        prime_list[prime_count++] = 2;
        prime_limit = 2;
    }

    /* Test every candidate for primality */
    for (idx_t cand = prime_limit + 1; cand <= limit; ++cand) {
        if ((cand & 1) == 0)                /* even numbers >2 cannot be prime */
            continue;

        bool composite = false;
        for (size_t i = 0; i < prime_count; ++i) {
            idx_t p = prime_list[i];
            if (p * p > cand)
                break;
            if (cand % p == 0) {
                composite = true;
                break;
            }
        }

        if (!composite) {     /* found a new prime */
            prime_list = (idx_t*)realloc(prime_list, (prime_count + 1) * sizeof(idx_t));
            prime_list[prime_count++] = cand;
        }
    }

    prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 2)           /* a(0) = a(1) = 1 by definition */
        return (seq_t)1;

    ensure_primes(n);

    seq_t prod = (seq_t)1;
    idx_t two_n = 2 * n;

    for (size_t i = 0; i < prime_count; ++i) {
        idx_t p = prime_list[i];
        if (p == 2)          /* omit the factor 2 */
            continue;
        if (p > n)
            break;          /* primes are sorted */

        /* Exponent of p in binomial(2n,n) via Legendre's formula */
        idx_t exp = 0;
        idx_t cur = p;
        while (cur <= two_n) {
            exp += (two_n / cur) - 2 * (n / cur);
            if (cur > two_n / p)   /* prevent overflow of cur *= p */
                break;
            cur *= p;
        }

        /* multiply prod by p^exp (exp is usually small) */
        for (idx_t e = 0; e < exp; ++e)
            prod *= (seq_t)p;
    }

    return prod;
}
