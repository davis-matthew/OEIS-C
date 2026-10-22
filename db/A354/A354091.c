/*
 * A354091  Fully multiplicative prime shift where the primes of the form 3k+2
 *          are replaced by the next larger such prime, and primes of the form
 *          3k and 3k+1 stay as they are.
 * https://oeis.org/A354091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 127, u32 n <= 8769, u64 n <= 8769,
 *            u128 n <= 8769, big any n
 * Helpers:   big_pow, build_primes_upto, int_sqrt, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * OEIS A354091: fully multiplicative prime shift where primes ≡2 (mod 3)
 * are mapped to the next larger prime of the same residue class, and all
 * other primes remain unchanged.
 * a(n) is computed by factorising n, mapping each prime factor, then
 * multiplying the mapped primes raised to their original exponents.
 */

/* Dependencies from the utils library */
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "../utils/build_primes_upto.h"
#include "../utils/int_sqrt.h"
#include "../utils/next_prime.h"
#include "../utils/big_pow.h"

/* Cached prime table for factorisation */
static bool primes_ready = false;
static uint64_t *primes   = NULL;   /* list of primes (as 64‑bit values)   */
static size_t  prime_cnt = 0;       /* number of primes in the list        */
static idx_t   prime_limit = 0;     /* highest value included in the list   */

/* Ensure that at least 'lim' is covered by the prime table */
static void ensure_primes(idx_t lim)
{
    if (primes_ready && lim <= prime_limit)
        return;
    free(primes);
    if (lim < 10)          /* small safety margin for tiny limits */
        lim = 10;
    build_primes_upto(lim, &primes, &prime_cnt);
    prime_limit = lim;
    primes_ready = true;
}

/* Temporary storage for the factorisation of the input index */
static uint64_t fac_pr[64];      /* distinct prime factors          */
static uint64_t fac_ex[64];      /* corresponding exponents         */
static size_t   fac_cnt;          /* number of distinct factors      */

/* Factorise n by trial division using the cached prime list */
static void factorise(uint64_t n)
{
    fac_cnt = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t p2 = p * p;
        if (p2 > n)          /* remaining n is prime               */
            break;
        if (n % p == 0) {
            uint64_t e = 0;
            while (n % p == 0) {
                n /= p;
                ++e;
            }
            fac_pr[fac_cnt]   = p;
            fac_ex[fac_cnt++] = e;
        }
    }
    if (n > 1) {              /* leftover prime factor (> sqrt)    */
        fac_pr[fac_cnt]   = n;
        fac_ex[fac_cnt++] = 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;      /* not part of the sequence, but harmless */
    if (n == 1)
        return (seq_t)1;

    /* Need primes up to sqrt(n) + a safety margin */
    idx_t needed = int_sqrt(n) + 10;
    ensure_primes(needed);

    factorise((uint64_t)n);

    seq_t result = (seq_t)1;

    for (size_t i = 0; i < fac_cnt; ++i) {
        uint64_t p   = fac_pr[i];
        uint64_t exp = fac_ex[i];

        /* Map the prime factor */
        uint64_t q;
        if (p % 3 == 2) {
            q = p;
            do {
                q = next_prime(q);
            } while (q % 3 != 2);
        } else {
            q = p;
        }

        /* Multiply q^exp into the result */
        seq_t pow_val = big_pow((seq_t)q, (seq_t)exp);
        result *= pow_val;
    }

    return result;
}
