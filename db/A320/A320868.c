/*
 * A320868  Primes such that p + digitsum(p, base 8) is again a prime.
 * https://oeis.org/A320868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 14, u16 n <= 788, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 9760 (2026-11-03)
 * Helpers:   is_prime, is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: primes p such that p + digit_sum(p, base 8) is also a prime
   Algorithm: sequentially generate primes, compute the base‑8 digit sum
   of each prime, test whether p + sum is prime, and store the qualifying
   primes in a static cache.  The cache is expanded lazily so that any
   call to a(n) obtains the term regardless of order of access. */

/* include required utilities */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"
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

/* local helper: allocate or grow a cache array */
static void ensure_cache_cap(size_t *cap, seq_t **arr, size_t needed) {
    if (*cap < needed) {
        size_t new_cap = (*cap == 0) ? 16 : (*cap << 1);
        if (new_cap < needed) new_cap = needed;
        *arr = mem_resize(seq_t, *arr, new_cap);
        *cap = new_cap;
    }
}

seq_t a(idx_t n) {
    /* static cache for already found terms */
    static seq_t *cache = NULL;   /* array of qualifying primes */
    static size_t cache_cap = 0;
    static size_t cache_size = 0; /* number of stored terms */
    static idx_t next_scan = 2;   /* number to start searching for the next prime */

    /* expand the cache until we have at least n terms */
    while (cache_size < n) {
        /* find next prime >= next_scan */
        idx_t prime_idx = next_prime(next_scan);
        next_scan = prime_idx + 1; /* advance for subsequent searches */

        /* prime found */
        seq_t p   = (seq_t)prime_idx;   /* convert to seq_t (may overflow if too large) */
        seq_t sum = (seq_t)0;           /* digit sum in base 8 */

        /* compute the digit sum in base 8 */
        seq_t t = p;
        while (t > (seq_t)0) {
            sum += t % (seq_t)8; /* modulo 8 is allowed on seq_t */
            t   /= (seq_t)8;
        }

        /* test whether p + sum is also prime */
        seq_t q = p + sum;
        if (is_prime_big(q)) {
            /* store the qualifying prime in the cache */
            ensure_cache_cap(&cache_cap, &cache, cache_size + 1);
            cache[cache_size++] = p;
        }
    }

    return cache[n - 1];
}
