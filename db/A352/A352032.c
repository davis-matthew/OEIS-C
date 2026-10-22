/*
 * A352032  Sum of the 4th powers of the odd proper divisors of n.
 * https://oeis.org/A352032
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{d|n, d<n, d odd} d^4
 * Range:     u8 n <= 9, u16 n <= 33, u32 n <= 513, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* For a(n) we compute σ_4(m) where m is the odd part of n.
   If n is odd we subtract n^4 to remove the divisor n itself. */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"
#include "../utils/_floor_isqrt_u64.h"

/* Prime list used for factorisation.  It grows as needed. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;
static idx_t  prime_limit = 0;

/* Make sure primes up to 'limit' (inclusive) are available. */
static void ensure_primes_up_to(idx_t limit)
{
    if (limit <= prime_limit)
        return;
    /* If we already have a list, free it.  The helper will allocate a new one. */
    if (prime_list)
        free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_cnt);
    prime_limit = limit;
}

/* Σ_{d|x} d^4 for an odd integer x (x ≥ 1).  Uses the multiplicative
   property of the divisor–sum function. */
static seq_t sigma_4_odd(uint64_t x)
{
    if (x == 1)
        return (seq_t)1;          /* σ_4(1) = 1 */

    seq_t result = 1;
    uint64_t remaining = x;

    /* Factorise x using primes up to √x. */
    idx_t sqrt_x = _floor_isqrt_u64(x);
    ensure_primes_up_to(sqrt_x);

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p > sqrt_x || remaining == 1)
            break;

        if (remaining % p == 0) {
            unsigned exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }

            seq_t p4 = (seq_t)p * (seq_t)p * (seq_t)p * (seq_t)p; /* p^4 */
            seq_t term = 1;    /* p^{4k} */
            seq_t sum   = 0;   /* Σ_{k=0..exp} p^{4k} */

            for (unsigned k = 0; k <= exp; ++k) {
                sum += term;
                term *= p4;
            }
            result *= sum;
        }
    }

    /* If a prime factor > √x remains, its exponent is 1. */
    if (remaining > 1) {
        seq_t p4 = (seq_t)remaining * (seq_t)remaining *
                  (seq_t)remaining * (seq_t)remaining;   /* remaining^4 */
        result *= (seq_t)1 + p4;
    }

    return result;
}

/* Main OEIS sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)        /* offset starts at 1; value for 0 is undefined – return 0 */
        return (seq_t)0;

    uint64_t nn = (uint64_t)n;

    /* Odd part of n. */
    uint64_t odd_part = nn;
    while ((odd_part & 1ULL) == 0)
        odd_part >>= 1;

    seq_t sigma = sigma_4_odd(odd_part);

    /* If n is odd, remove the divisor n itself. */
    if ((nn & 1ULL) != 0) {
        seq_t n4 = (seq_t)nn * (seq_t)nn;
        n4 *= n4;                       /* n^4 */
        sigma -= n4;                    /* guarantees non‑negative by definition */
    }

    return sigma;
}
