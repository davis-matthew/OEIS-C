/*
 * A289750  a(1) = 3 and a(n+1) - a(n) = 2*p where p is the least possible
 *          prime.
 * https://oeis.org/A289750
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 31, u16 n <= 2315, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 76 terms through n = 9421 (2026-11-03)
 * Helpers:   build_primes_upto, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A289750:
 * a(1) = 3
 * a(n+1) = a(n) + 2*p
 * where p is the smallest prime such that a(n)+2*p is prime. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"
#include "../utils/mem.h"
#include "../utils/is_prime_u64.h"

/* Miller–Rabin primality test for seq_t (supports 8/16/32/64/128/ arbitrary-precision) */
/* n fits in uint64_t (n >= 0 here) */
static bool is_prime_big_fits_u64(seq_t v)
{
    if (sizeof(seq_t) <= 8) return true;
    return !((seq_t)UINT64_MAX < v);
}

static bool is_prime_big(seq_t n) {
    if (n < 2) return false;
    /* exact in 64 bits when n fits: the seq_t products below overflow at
       u64/i64 once n passes 2^32, and are slow under big */
    if (is_prime_big_fits_u64(n)) return is_prime_u64((uint64_t)n);
    if (n == 2 || n == 3) return true;
    if ((n & 1) == 0) return false;

    /* write n-1 as d * 2^s with d odd */
    seq_t d = n - 1;
    unsigned s = 0;
    while ((d & 1) == 0) {
        d >>= 1;
        s++;
    }

    /* deterministic bases covering 128‑bit values */
    static const int base_list[] = {2,3,5,7,11,13,17,19,23,29,31,37};
    const int num_bases = sizeof(base_list)/sizeof(base_list[0]);

    for (int i = 0; i < num_bases; i++) {
        seq_t a = (seq_t)base_list[i];
        if (a >= n) continue;

        /* compute a^d mod n */
        seq_t x = 1;
        seq_t pow = a % n;
        seq_t exp = d;
        while (exp > 0) {
            if (exp & 1) x = (x * pow) % n;
            pow = (pow * pow) % n;
            exp >>= 1;
        }

        if (x == 1 || x == n-1) continue;

        bool cont = false;
        for (unsigned r = 1; r < s; r++) {
            x = (x * x) % n;
            if (x == n-1) { cont = true; break; }
        }
        if (!cont) return false;
    }
    return true;
}

/* Cache of already‑computed values.  cache[1] holds a(1). */
static seq_t *cache      = NULL;   /* cache[0] is unused */
static idx_t cache_len   = 0;      /* allocated length (>= current max index) */
static idx_t computed    = 1;      /* number of terms already computed */

/* Prime list used to search for the required p. */
static uint64_t *prime_arr   = NULL;   /* array of primes (uint64_t) */
static size_t   prime_cnt    = 0;      /* number of primes in prime_arr   */
static idx_t    prime_limit  = 0;      /* largest limit for which primes
                                        * have been generated            */

/* Grow the cache so that indices up to n (inclusive) are stored. */
static void expand_cache(idx_t n)
{
    if (cache == NULL) {
        /* Allocate with a few spare slots. */
        cache_len = n + 10;
        cache = mem_new(seq_t, (size_t)(cache_len + 1)); /* cache[0] unused */
        if (cache == NULL) abort();
        cache[1] = (seq_t)3;   /* a(1) = 3 */
        computed = 1;
    } else if (n > cache_len) {
        idx_t newlen = n + 10;
        seq_t *tmp = mem_new(seq_t, (size_t)(newlen + 1));
        if (tmp == NULL) abort();
        mem_copy(tmp, cache, (size_t)(cache_len + 1) * sizeof(seq_t));
        mem_free(cache);
        cache = tmp;
        cache_len = newlen;
    }
}

/* Build a list of all primes ≤ limit.  Old prime list (if any) is freed. */
static void build_primes_for(idx_t limit)
{
    if (prime_arr) free(prime_arr);
    uint64_t *tmp = NULL;
    size_t    cnt = 0;
    build_primes_upto(limit, &tmp, &cnt);     /* tmp points to an array of uint64_t */
    prime_arr   = tmp;
    prime_cnt   = cnt;
    prime_limit = limit;
}

/* Ensure that cache contains terms up to index n (inclusive). */
static void ensure_cache_up_to(idx_t n)
{
    if (n < 1) return;

    expand_cache(n);

    /* At first call we need a small prime list. */
    if (prime_arr == NULL) build_primes_for(100);

    while (computed < n) {
        seq_t prev   = cache[computed];
        bool found   = false;

        for (size_t i = 0; i < prime_cnt; ++i) {
            uint64_t p   = prime_arr[i];
            seq_t cand   = prev + ((seq_t)2) * ((seq_t)p);
            if (is_prime_big(cand)) {
                cache[computed + 1] = cand;
                ++computed;
                found = true;
                break;
            }
        }

        if (!found) {
            /* No suitable prime found in current list – enlarge the list. */
            idx_t new_limit = prime_limit < 10 ? 20 : prime_limit * 2;
            build_primes_for(new_limit);
            /* loop will retry this index with the larger prime list. */
        }
    }
}

/* Public API */
seq_t a(idx_t n)
{
    ensure_cache_up_to(n);
    return cache[n];
}
