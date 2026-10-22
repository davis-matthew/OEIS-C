/*
 * A349175  Odd numbers k for which gcd(k, A003961(k)) <> gcd(sigma(k),
 *          A003961(k)), where A003961(n) is fully multiplicative with
 *          a(prime(k)) = prime(k+1), and sigma is the sum of divisors
 *          function.
 * https://oeis.org/A349175
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   _smallest_prime_factor, gcd, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search-based computation of A349175:
 * odd k for which gcd(k, A003961(k)) <> gcd(sigma(k), A003961(k))
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/gcd.h"
#include "../utils/_smallest_prime_factor.h"
#include "../utils/next_prime.h"

/* Cached sequence values and state */
static seq_t *seq_cache   = NULL;   /* array of terms */
static size_t seq_len     = 0;       /* number of terms stored */
static size_t seq_cap     = 0;       /* allocated capacity */

/* Current odd candidate to test */
static uint64_t current_candidate = 15;  /* first odd with property */

/* Test whether an odd integer k satisfies the defining condition */
static bool in_sequence(uint64_t k)
{
    /* Factorise k using the smallest‑prime‑factor routine */
    uint64_t tmp = k;
    uint64_t primes[64];
    int      exps[64];
    size_t   nf = 0;             /* number of distinct prime factors */

    while (tmp > 1 && nf < 64) {
        uint64_t p = _smallest_prime_factor(tmp);
        int      e = 0;
        while (tmp % p == 0) {
            tmp /= p;
            ++e;
        }
        primes[nf] = p;
        exps[nf]   = e;
        ++nf;
    }
    if (tmp > 1) {               /* remaining prime factor larger than 2^32 */
        primes[nf] = tmp;
        exps[nf]   = 1;
        ++nf;
    }

    /* Compute A003961(k) and sigma(k) from the factorisation */
    seq_t a003961   = 1;
    seq_t sigma_val = 1;

    for (size_t i = 0; i < nf; ++i) {
        uint64_t p = primes[i];
        int      e = exps[i];

        /* A003961 part : product of next_prime(p)^e */
        uint64_t nextp = next_prime(p);
        seq_t    pow_next = 1;
        for (int j = 0; j < e; ++j)
            pow_next *= (seq_t)nextp;
        a003961 *= pow_next;

        /* sigma part : product of (1 + p + ... + p^e) */
        seq_t sum_powers = 1;      /* includes the 1 */
        seq_t pow_p = 1;
        for (int j = 1; j <= e; ++j) {
            pow_p   *= (seq_t)p;
            sum_powers += pow_p;
        }
        sigma_val *= sum_powers;
    }

    seq_t g1 = gcd((seq_t)k,     a003961);
    seq_t g2 = gcd(sigma_val,    a003961);
    return g1 != g2;
}

/* Ensure that seq_cache has at least n entries */
static void build_cache_to(idx_t n)
{
    while (seq_len < (size_t)n) {
        if (in_sequence(current_candidate)) {
            if (seq_len == seq_cap) {
                size_t new_cap = seq_cap == 0 ? 4 : seq_cap * 2;
                seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();      /* allocation failure – should not happen */
                seq_cache = tmp;
                seq_cap   = new_cap;
            }
            seq_cache[seq_len++] = (seq_t)current_candidate;
        }
        current_candidate += 2;          /* next odd integer */
    }
}

/* Public entry point required by the harness */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* undefined index below offset */
    build_cache_to(n);
    return seq_cache[n - 1];
}
