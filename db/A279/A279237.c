/*
 * A279237  Let k_i be the multiplicity of prime(i) in the prime factorization
 *          of the n-th composite number C_n, and let k_i=0 if prime(i) is not
 *          a factor of C_n. Then a(n)=1*k_1+10*k_2+100*k_3+...+10^N*k_N,
 *          where N is the index of the largest prime factor in C_n.
 * https://oeis.org/A279237
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Σ_i k_i · 10^(i‑1) where k_i is the exponent of the i‑th
 *            prime in the factorisation of the n‑th composite number
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 42, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   build_primes_upto, factor_u64, is_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* OEIS supplied utilities */
#include "../utils/is_prime.h"
#include "../utils/pow10.h"
#include "../utils/build_primes_upto.h"
#include "../utils/factor_u64.h"

/* ----- Prime storage ----- */
static uint64_t *primes = NULL;      /* list of primes in ascending order   */
static size_t primes_cnt = 0;        /* number of primes stored              */
static bool primes_initialised = false;

/* Ensure the prime table is built once, up to an upper bound that is
   sufficient for the expected range of composites (n up to ~10000). */
static void ensure_primes(void)
{
    if (primes_initialised)
        return;
    /* 200 000 covers the 10 000‑th composite comfortably. */
    idx_t limit = 200000;
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_initialised = true;
}

/* Binary search to find the 1‑based index of a prime `p`.
   It is guaranteed that `p` is prime and <= primes[primes_cnt‑1]. */
static size_t prime_index(uint64_t p)
{
    size_t lo = 0, hi = primes_cnt;
    while (lo < hi) {
        size_t mid = (lo + hi) >> 1;
        if (primes[mid] < p)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo + 1;   /* convert to 1‑based index */
}

/* ----- Cache for computed a(n) values ----- */
static seq_t *cache = NULL;
static size_t cache_cap = 0;
static idx_t cache_len = 0;   /* number of values stored in cache */

/* Guarantee the cache can hold at least `need` elements. */
static void ensure_cache_capacity(idx_t need)
{
    if ((size_t)need <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while ((size_t)need > new_cap)
        new_cap <<= 1;
    seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_arr)
        exit(1);
    cache = new_arr;
    cache_cap = new_cap;
}

/* ----- Main sequence function ----- */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                      /* undefined for offset 1, harmless */

    /* Lazily compute terms up to n. */
    static uint64_t last_comp = 3;     /* start just below first composite 4 */
    while (cache_len < n) {
        /* Find next composite number > last_comp. */
        uint64_t cand = last_comp + 1;
        while (is_prime(cand))
            ++cand;

        /* Prepare prime infrastructure. */
        ensure_primes();

        /* Factorise the composite number. */
        uint64_t fp[64];
        uint64_t fe[64];
        size_t fp_cnt = 0;
        factor_u64(cand, fp, fe, &fp_cnt);   /* primes ascending */

        /* Convert factorisation into the required decimal number. */
        seq_t term = 0;
        seq_t powTen = 1;          /* 10^(i‑1) for current i */
        size_t lastIdx = 0;        /* 1‑based index of last used prime */

        for (size_t i = 0; i < fp_cnt; ++i) {
            uint64_t p   = fp[i];
            size_t idx   = prime_index(p);           /* 1‑based */
            size_t missing = idx - lastIdx - 1;
            if (missing)
                powTen *= pow10((idx_t)missing);    /* skip gaps */
            term += powTen * (seq_t)fe[i];
            powTen *= 10;                           /* next decade */
            lastIdx = idx;
        }

        /* Store the computed term. */
        ensure_cache_capacity(cache_len + 1);
        cache[cache_len++] = term;

        /* Update state for next iteration. */
        last_comp = cand;
    }

    /* Cache is zero‑based whereas user index is one‑based. */
    return cache[n - 1];
}
