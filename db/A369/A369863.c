/*
 * A369863  Inert rational primes in the field Q(sqrt(-21)).
 * https://oeis.org/A369863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th inert prime in Q(sqrt(-21)) by generating primes
   and testing Legendre(-21,p) = -1.  The sequence is indexed from 1.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* External helpers */
#include "../utils/jacobi_symbol.h"

typedef uint64_t prim_t;   /* type used for intermediate prime generation */

/* Memoised inert primes */
static seq_t *inert = NULL;
static size_t inert_len = 0;
static size_t inert_cap = 0;
static prim_t last_checked = 3;   /* start searching from 3 */

/* Memoised small prime list for primality testing */
static prim_t *primes = NULL;
static size_t primes_len = 0;
static size_t primes_cap = 0;

/* Ensure inert array has capacity at least 'needed' */
static void ensure_inert_cap(size_t needed)
{
    if (needed <= inert_cap) return;
    size_t new_cap = inert_cap ? inert_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    inert = (seq_t *)realloc(inert, new_cap * sizeof(seq_t));
    inert_cap = new_cap;
}

/* Ensure primes array has capacity at least 'needed' */
static void ensure_primes_cap(size_t needed)
{
    if (needed <= primes_cap) return;
    size_t new_cap = primes_cap ? primes_cap << 1 : 16;
    while (new_cap < needed) new_cap <<= 1;
    primes = (prim_t *)realloc(primes, new_cap * sizeof(prim_t));
    primes_cap = new_cap;
}

/* Test primality of p using the current list of small primes.
   The list contains all primes < sqrt(p). */
static bool is_prime_candidate(prim_t p)
{
    /* Add p to the primes list if it is prime */
    if (p < 2) return false;
    if (p == 2) {
        ensure_primes_cap(primes_len + 1);
        primes[primes_len++] = 2;
        return true;
    }
    if ((p & 1U) == 0) return false;

    // Ensure we have all primes up to sqrt(p)
    prim_t limit = 0;
    while ((limit + 1) * (limit + 1) <= p) limit++;
    for (prim_t candidate = primes_len ? primes[primes_len - 1] + 2 : 3;
         candidate <= limit;
         candidate += 2)
    {
        if (candidate < 2) continue;
        // skip even
        bool prime_candidate = true;
        for (size_t i = 0; i < primes_len; ++i) {
            prim_t q = primes[i];
            if (q * q > candidate) break;
            if (candidate % q == 0) { prime_candidate = false; break; }
        }
        if (prime_candidate) {
            ensure_primes_cap(primes_len + 1);
            primes[primes_len++] = candidate;
        }
    }

    /* Now test p */
    for (size_t i = 0; i < primes_len; ++i) {
        prim_t q = primes[i];
        if (q * q > p) break;
        if (p % q == 0) return false;
    }
    return true;
}

/* Legendre symbol (−21/p) using Jacobi symbol and (-1/p) factor. */
static int legendre_minus21(prim_t p)
{
    /* p must be odd prime not dividing 21 */
    int s21 = jacobi_symbol(21ULL, p);                /* (21/p) */
    int sneg = ((p & 3ULL) == 1ULL) ? 1 : -1;         /* (-1/p) */
    return s21 * sneg;                               /* (−21/p) */
}

/* Return n-th inert prime (1-indexed). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined; harmless for callers expecting >=1 */

    /* Generate inert primes until we have n of them */
    while (inert_len < (size_t)n)
    {
        /* Find next prime candidate */
        while (!is_prime_candidate(last_checked)) {
            last_checked += (last_checked == 2) ? 1 : 2; /* 2 -> 3, then odd steps */
        }

        /* Skip primes dividing 21 */
        if ((last_checked % 3ULL) == 0ULL || (last_checked % 7ULL) == 0ULL) {
            last_checked += 2;
            continue;
        }

        /* Test Legendre symbol */
        if (legendre_minus21(last_checked) == -1)
        {
            ensure_inert_cap(inert_len + 1);
            inert[inert_len++] = (seq_t)last_checked;
        }

        last_checked += 2;
    }

    return inert[n - 1];
}
