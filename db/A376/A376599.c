/*
 * A376599  Second differences of consecutive non-prime-powers inclusive
 *          (A024619). First differences of A375735.
 * https://oeis.org/A376599
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 80, i16 n <= 80, i32 n <= 80, i64 n <= 80, i128 n <= 80,
 *            big any n
 * Helpers:   build_primes_upto, ensure_capacity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = b(n+2) - 2*b(n+1) + b(n)
 * where b(k) is the k-th non-prime-power (inclusive) (A024619).
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/build_primes_upto.h"
#include "../utils/ensure_capacity.h"

static seq_t *b_cache = NULL;        /* cache of non‑prime‑powers */
static size_t b_cache_len = 0;       /* number of elements stored */
static size_t b_cache_cap = 0;       /* capacity of cache */

static uint64_t *primes = NULL;      /* array of primes for factorization */
static size_t primes_count = 0;      /* number of primes */
static bool primes_built = false;    /* flag indicating if primes are prepared */

/* Helper: test if n is a prime power (n >= 2). 1 is treated as not a prime‑power here. */
static bool is_prime_power(uint64_t n)
{
    if (n < 2)
        return false;

    uint64_t temp = n;
    uint64_t spf = 0; /* smallest prime factor seen */

    for (size_t idx = 0; idx < primes_count && primes[idx] * primes[idx] <= temp; ++idx) {
        uint64_t p = primes[idx];
        if (temp % p == 0) {
            if (spf == 0)
                spf = p;
            else if (spf != p)
                return false;         /* more than one distinct prime factor */
            while (temp % p == 0)
                temp /= p;
        }
    }
    if (temp > 1) {           /* remaining factor > sqrt(original n) */
        if (spf == 0)
            spf = temp;
        else if (spf != temp)
            return false;
        temp = 1;
    }
    return true;
}

/* Ensure the non-prime-power cache contains at least 'need' elements (1-indexed). */
static void ensure_b_cache(idx_t need)
{
    if (!primes_built) {
        /* Build enough primes for any candidate up to a safe limit.
           200000 is more than enough for the required range. */
        build_primes_upto((idx_t)200000, &primes, &primes_count);
        primes_built = true;
    }

    if (b_cache_len >= (size_t)need)
        return;

    /* Find next candidates until we reach 'need'. */
    static uint64_t candidate = 2;  /* start checking from 2 */
    while (b_cache_len < (size_t)need) {
        if (!is_prime_power(candidate)) {
            /* Append candidate to b_cache */
            ensure_capacity(b_cache_len + 1, &b_cache_cap,
                            (void **)&b_cache, sizeof(seq_t));
            b_cache[b_cache_len] = (seq_t)candidate;
            ++b_cache_len;
        }
        ++candidate;
    }
}

/* Public sequence function */
seq_t a(idx_t n)
{
    /* We need b(n), b(n+1), b(n+2) => indices 0 .. n+1 in 0‑based cache */
    idx_t needed = n + 2;               /* number of b entries required */
    ensure_b_cache(needed);

    seq_t b_n   = b_cache[n - 1];
    seq_t b_n1  = b_cache[n];
    seq_t b_n2  = b_cache[n + 1];

    return b_n2 - (seq_t)2 * b_n1 + b_n;
}
