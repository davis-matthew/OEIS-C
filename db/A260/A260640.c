/*
 * A260640  Numbers n such that binomial(3*n,n) == 0 (mod n).
 * https://oeis.org/A260640
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth integer m ≥ 1 such that binomial(3*m, m) ≡ 0
 *            (mod m)
 * Range:     u8 n <= 37, u16 n <= 8192, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>

/*--- prime utilities ---------------------------------------------------- */
static uint64_t *prime_list = NULL;
static size_t prime_count = 0;
static uint64_t last_checked = 1;

/* extend the prime list up to at least 'limit' */
static void extend_primes(uint64_t limit) {
    if (limit <= last_checked) return;
    for (uint64_t cand = last_checked + 1; cand <= limit; ++cand) {
        bool is_prime = true;
        for (size_t i = 0; i < prime_count && prime_list[i] * prime_list[i] <= cand; ++i) {
            if (cand % prime_list[i] == 0) { is_prime = false; break; }
        }
        if (is_prime) {
            prime_list = (uint64_t*)realloc(prime_list, (prime_count + 1) * sizeof(uint64_t));
            prime_list[prime_count++] = cand;
        }
    }
    last_checked = limit;
}

/*--- factorial exponent in a binomial --------------------------------- */
static uint64_t exp_in_binom(uint64_t n, uint64_t p) {
    uint64_t e = 0;
    unsigned __int128 pk = p;
    unsigned __int128 three_n = (unsigned __int128)3 * n;
    unsigned __int128 two_n   = (unsigned __int128)2 * n;
    while (pk <= three_n) {
        e += (uint64_t)(three_n / pk);
        e -= n / (uint64_t)pk;
        e -= (uint64_t)(two_n / pk);
        pk *= p;
    }
    return e;
}

/*--- caching of computed terms ----------------------------------------- */
static seq_t *cache = NULL;
static size_t cache_size = 0;
static size_t cache_cap  = 0;

static void ensure_cache_cap(size_t need) {
    if (need <= cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 4 : cache_cap * 2;
    while (new_cap < need) new_cap *= 2;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

/*--- main sequence function -------------------------------------------- */
seq_t a(idx_t n) {
    if (n == 0) return 0;                      /* undefined for offset 1, but safe */
    /* extend prime list to at least sqrt(max_candidate). We'll grow only when needed. */
    static uint64_t next_candidate = 1;         /* next integer to test */
    while (cache_size < (size_t)n) {
        /* ensure prime list long enough for factorization of next_candidate */
        uint64_t limit = 1;
        while (limit * limit < next_candidate) limit <<= 1;
        extend_primes(limit);

        /* factorize next_candidate and test the congruence */
        uint64_t m = next_candidate;
        bool ok = true;
        uint64_t temp = m;
        for (size_t idx = 0; idx < prime_count && prime_list[idx] * prime_list[idx] <= temp; ++idx) {
            uint64_t p = prime_list[idx];
            if (m % p == 0) {
                uint64_t exp_m = 0;
                while (m % p == 0) { m /= p; ++exp_m; }
                uint64_t exp_binom = exp_in_binom(temp, p);
                if (exp_binom < exp_m) { ok = false; break; }
            }
        }
        if (ok && m > 1) { /* remaining prime factor */
            uint64_t p = m;
            uint64_t exp_m = 1; /* because m > sqrt(temp) => exponent is 1 */
            uint64_t exp_binom = exp_in_binom(temp, p);
            if (exp_binom < exp_m) ok = false;
        }

        if (ok) {
            ensure_cache_cap(cache_size + 1);
            cache[cache_size++] = (seq_t)temp;
        }
        ++next_candidate;
    }
    return cache[n - 1];
}
