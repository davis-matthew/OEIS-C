/*
 * A276462  Prime numbers that consist of k 2's digits followed by k+1 1's
 *          digits for some k >= 1.
 * https://oeis.org/A276462
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 2, u64 n <= 3, u128 n <= 3,
 *            big any n
 * Helpers:   is_prime_big, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes the OEIS sequence A276462.
   Each term is the smallest prime whose decimal representation
   consists of k copies of the digit '2' followed by k+1 copies of
   the digit '1' for some k ≥ 1.
   The function memoises discovered primes so that calls in any
   order remain fast. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"
#include "../utils/pow10.h"

/* Cached primes: prime_cache[0] == a(1), prime_cache[1] == a(2), ... */
static seq_t *prime_cache = NULL;
static size_t cache_len = 0, cache_cap = 0;
/* The value of k that was last examined. */
static idx_t last_k = 0;

/* Return the nth term of the sequence (1‑based index). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* not used; offset starts at 1 */

    /* Generate primes until we have the first n terms. */
    while (cache_len < n)
    {
        /* Next k to test. */
        ++last_k;

        /* 10^k and 10^(k+1). */
        seq_t pow_k  = pow10(last_k);
        seq_t pow_k1 = pow10(last_k + 1);

        /* (10^k - 1) / 9 and (10^(k+1)-1) / 9.  These are k‑digit and (k+1)-digit
           repunits of '1'. */
        seq_t Rk   = (pow_k - ((seq_t)1)) / ((seq_t)9);
        seq_t Rk1  = (pow_k1 - ((seq_t)1)) / ((seq_t)9);

        /* Candidate: 2·Rk followed by k+1 ones.  Build by
           2*Rk * 10^(k+1) + Rk1. */
        seq_t candidate = ((seq_t)2) * Rk * pow_k1 + Rk1;

        /* Test primality.  The provided utility implements an arbitrary-precision
           Miller‑Rabin or equivalent test. */
        if (is_prime_big(candidate))
        {
            /* Grow the cache if necessary. */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(prime_cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(1);      /* Out of memory. */
                prime_cache = tmp;
                cache_cap = new_cap;
            }
            prime_cache[cache_len++] = candidate;
        }
    }

    /* Return the (n‑1)‑th cached prime. */
    return prime_cache[n - 1];
}
