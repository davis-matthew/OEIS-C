/*
 * A321422  Primes that are non-Chen primes whose reversal is a Chen prime.
 * https://oeis.org/A321422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth prime p such that (1) p is a non‑Chen prime (p+2
 *            is neither prime nor semiprime) (2) rev(p) is a Chen prime
 *            (rev(p)+2 is prime or semiprime). The function searches
 *            successive primes and caches the qualifying ones
 * Range:     u8 n <= 1, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   build_primes, is_prime_u128, is_prime_u64, mem, reverse_digits_idx, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>

#include <limits.h>
#include "../utils/build_primes.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/reverse_digits_idx.h"
#include "../utils/smallest_prime_factor.h"
#include "../utils/mem.h"

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

/* Test whether n (<= 2^64) is a semiprime (product of two primes). */
static bool is_semiprime(uint64_t n)
{
    if (n < 4) return false;                 /* must be ≥ 2*2 */
    uint64_t spf = smallest_prime_factor(n);
    if (spf == 0 || spf == n) return false;   /* prime or error */
    uint64_t q = n / spf;
    if (q < 2) return false;
    return isPrimeGeneric((seq_t)q);          /* second factor must be prime */
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;                                 /* undefined */

    /* ------- static state: cache of sequence terms -------- */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;                        /* number of stored terms */
    static size_t cache_cap  = 0;                        /* allocated capacity */

    /* ------- static state: prime list for searching -------- */
    static idx_t *prime_list = NULL;                     /* primes up to sieve_limit */
    static size_t prime_cnt   = 0;                        /* number of primes */
    static idx_t sieve_limit  = 0;                        /* current sieve upper bound */
    static idx_t prime_pos    = 0;                        /* next prime to process */

    /* Initialise cache on first call. */
    if (cache == NULL) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) return 0; /* allocation failure */
    }

    /* Initialise prime list on first call. */
    if (prime_list == NULL) {
        sieve_limit = 1000;                                 /* start with a small limit */
        build_primes(sieve_limit, &prime_list, &prime_cnt);
        prime_pos = 0;
    }

    /* If the requested term is already cached, return it. */
    if ((idx_t)n <= cache_len) return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        /* If we have exhausted the current list, extend it. */
        if (prime_pos >= prime_cnt) {
            uint64_t new_lim = (uint64_t)sieve_limit * 2;
            idx_t *new_list = NULL;
            size_t new_cnt  = 0;
            build_primes(new_lim, &new_list, &new_cnt);
            free(prime_list);
            prime_list = new_list;
            prime_cnt  = new_cnt;
            sieve_limit = new_lim;
            /* prime_pos remains unchanged: it already points past the
               primes we have processed. */
        }

        /* Candidate prime. */
        idx_t p = prime_list[prime_pos++];
        /* Reverse of p. */
        seq_t revp = (seq_t)reverse_digits_idx(p);

        /* Check that revp is a Chen prime. */
        seq_t rev_plus = revp + (seq_t)2;
        bool rev_chen = isPrimeGeneric(rev_plus) || is_semiprime((uint64_t)rev_plus);
        if (!rev_chen) continue;
        if (!isPrimeGeneric(revp)) continue;   /* revp must itself be prime */

        /* Check that p is a non‑Chen prime. */
        seq_t p_plus = (seq_t)p + (seq_t)2;
        bool non_chen = !isPrimeGeneric(p_plus) && !is_semiprime((uint64_t)p_plus);
        if (!non_chen) continue;

        /* Store the term in cache. */
        if (cache_len == cache_cap) {
            cache_cap <<= 1;
            seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
            if (!tmp) { mem_free(cache); return 0; }
            cache = tmp;
        }
        cache[cache_len++] = (seq_t)p;
    }

    return cache[n - 1];
}
