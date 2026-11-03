/*
 * A259190  Primes of the form sigma(n) + sigma(n)^2 - 1.
 * https://oeis.org/A259190
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 54, u32 n <= 3843, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9798 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A259190: a(n) is the n-th prime of the form σ(k) + σ(k)^2 – 1,
   where σ(k) is the sum of divisors of k.  The primes are collected
   in the order of increasing k. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

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

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Return the n‑th term of the sequence for n >= 1. */
seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
     *  Static state for memoisation
     * -------------------------------------------------------------------- */
    static seq_t *primes      = NULL;      /* found primes  */
    static size_t primes_len  = 0;          /* number of stored primes */
    static size_t primes_cap  = 0;          /* allocated capacity     */

    static idx_t current_k = 0;             /* last k processed       */

    /* ------------------------------------------------------------
     *  Grow the prime array to accommodate at least n primes
     * ------------------------------------------------------------ */
    while (primes_len < n) {
        /* --- next k to test --- */
        ++current_k;

        /* --- compute candidate --- */
        seq_t sigma = sigma_of(current_k);
        seq_t candidate = sigma * sigma + sigma - (seq_t)1;

        /* --- test primality --- */
        if (isPrimeGeneric(candidate)) {
            /* grow primes array if necessary */
            if (primes_len == primes_cap) {
                size_t new_cap = primes_cap ? primes_cap << 1 : 64;
                seq_t *tmp = mem_resize(seq_t, primes, new_cap);
                if (!tmp) {
                    /* OOM: return 0 (undefined behaviour not required to be handled) */
                    mem_free(primes);
                    primes = NULL;
                    primes_cap = primes_len = 0;
                    return (seq_t)0;
                }
                primes = tmp;
                primes_cap = new_cap;
            }
            primes[primes_len++] = candidate;
        }
    }

    return primes[n - 1];
}
