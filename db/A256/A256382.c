/*
 * A256382  Numbers n such that n-4 and n+4 are semiprimes.
 * https://oeis.org/A256382
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   seq_sqrt_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A256382 – Numbers n such that n-4 and n+4 are semiprimes. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_sqrt_floor.h"

/* --------------------------------------------------------------------- */
/* Utility: integer square root of an arbitrary precision number seq_t  */

/* --------------------------------------------------------------------- */
/* Simple incremental sieve for small primes (uint64_t).                  */
/* --------------------------------------------------------------------- */
#define MAX_PRIMES  65536

static uint64_t  g_primes[ MAX_PRIMES ];
static size_t    g_prime_cnt  = 0;
static uint64_t  g_prime_max  = 1;   /* largest number tested for primality */

static void
ensure_primes_up_to(uint64_t limit)
{
    if (limit <= g_prime_max)
        return;

    for (uint64_t n = g_prime_max + 1; n <= limit; ++n) {
        bool is_prime = true;
        for (size_t i = 0; i < g_prime_cnt; ++i) {
            uint64_t p = g_primes[i];
            if (p * p > n)
                break;
            if (n % p == 0) {
                is_prime = false;
                break;
            }
        }
        if (is_prime) {
            if (g_prime_cnt == MAX_PRIMES)
                return;          /* should not happen for needed limits */
            g_primes[g_prime_cnt++] = n;
        }
    }
    g_prime_max = limit;
}

/* --------------------------------------------------------------------- */
/* Test whether a number x (seq_t) is a semiprime.                        */
/* --------------------------------------------------------------------- */
static bool
is_semiprime(seq_t x)
{
    if (x < (seq_t)4)
        return false;

    seq_t sqrt_x = seq_sqrt_floor(x);
    uint64_t sqrt_x_u = (uint64_t)sqrt_x;   /* will fit in 64‑bit for the needed range */
    ensure_primes_up_to(sqrt_x_u);

    for (size_t i = 0; i < g_prime_cnt &&
                      (seq_t)g_primes[i] <= sqrt_x; ++i)
    {
        seq_t p = (seq_t)g_primes[i];
        if (x % p == 0) {
            seq_t q = x / p;
            /* Check whether q is prime.  q is guaranteed to be <= sqrt(x) * sqrt(x) = x,
               but typically <= sqrt(x)^2, so it fits in 64‑bit for the range we care about. */
            seq_t sqrt_q = seq_sqrt_floor(q);
            bool q_prime = true;
            for (size_t j = 0; j < g_prime_cnt &&
                                 (seq_t)g_primes[j] <= sqrt_q; ++j)
            {
                seq_t p2 = (seq_t)g_primes[j];
                if (q % p2 == 0) {
                    q_prime = false;
                    break;
                }
            }
            return q_prime;
        }
    }
    return false;
}

/* --------------------------------------------------------------------- */
/* Storage for the sequence values.                                      */
/* --------------------------------------------------------------------- */
static seq_t  *g_terms   = NULL;
static size_t  g_len     = 0;
static size_t  g_cap     = 0;

/* Append a new term to the cached array, growing it if necessary. */
static void
append_term(seq_t v)
{
    if (g_cap == 0) {
        g_cap   = 16;
        g_terms = (seq_t *)malloc(g_cap * sizeof(seq_t));
    } else if (g_len == g_cap) {
        g_cap   = g_cap * 2;
        g_terms = (seq_t *)realloc(g_terms, g_cap * sizeof(seq_t));
    }
    g_terms[g_len++] = v;
}

/* --------------------------------------------------------------------- */
/* Public function: return the nth element of A256382 (n ≥ 1).           */
/* --------------------------------------------------------------------- */
static idx_t  g_next_candidate = 5;   /* first number to test */

seq_t a(idx_t n)
{
    /* Ensure at least n terms are computed. */
    while (g_len < (size_t)n) {
        seq_t m    = (seq_t)g_next_candidate;
        seq_t m1 = m - (seq_t)4;
        seq_t m2 = m + (seq_t)4;
        if (is_semiprime(m1) && is_semiprime(m2))
            append_term(m);
        ++g_next_candidate;
    }
    return g_terms[n - 1];
}
