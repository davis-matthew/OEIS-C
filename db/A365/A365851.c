/*
 * A365851  The number of divisors of the n-th practical number (A005153).
 * https://oeis.org/A365851
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the number of divisors of the n-th practical number
 *            (A005153). The function generates practical numbers on demand
 *            and counts their divisors. It works for any build-defined
 *            seq_t/idx_t
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   divisor_count_u64, seq_realloc, smallest_prime_factor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/smallest_prime_factor.h"
#include "../utils/divisor_count_u64.h"
#include "../utils/seq_realloc.h"

/* Cache of previously computed values. */
static seq_t *cache = NULL;
static size_t cache_size = 0;   /* number of stored terms          */
static size_t cache_cap  = 0;   /* allocated capacity              */
static uint64_t last_practical = 0;   /* last practical number found   */

/* Test whether m is a practical number. */
static bool is_practical(uint64_t m)
{
    if (m == 1) return true;

    unsigned __int128 sigma = 1;   /* σ of the processed part */
    uint64_t n = m;

    while (n > 1) {
        uint64_t p = smallest_prime_factor(n);
        uint64_t e = 0;
        while (n % p == 0) { n /= p; ++e; }

        if (p > (uint64_t)(sigma + 1)) return false;

        /* σ(p^e) = (p^(e+1)-1)/(p-1) */
        unsigned __int128 p_pow = 1;
        for (uint64_t i = 0; i < e; ++i) p_pow *= p;
        unsigned __int128 sigma_p = (p_pow * p - 1) / (p - 1);
        sigma *= sigma_p;
    }
    return true;
}

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_cache(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap << 1 : 8;
    while (new_cap < needed) new_cap <<= 1;

    cache = seq_realloc(cache, cache_cap, new_cap);
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n<1, but safe */

    /* Generate terms up to n. */
    while (cache_size < (size_t)n) {
        uint64_t candidate = last_practical + 1;
        while (!is_practical(candidate)) ++candidate;

        seq_t tau = divisor_count_u64(candidate);

        ensure_cache(cache_size + 1);
        cache[cache_size++] = tau;
        last_practical = candidate;
    }
    return cache[n - 1];
}
