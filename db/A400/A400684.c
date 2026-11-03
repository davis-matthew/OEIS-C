/*
 * A400684  Primes p such that p+2 and (p-1)^2+1 are also prime.
 * https://oeis.org/A400684
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, new
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Validated: partial, pass, 46 terms through n = 46 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Computes OEIS A400684: the primes p such that p+2 and (p-1)^2+1 are also prime.
 * The algorithm incrementally tests successive odd integers for primality (using the
 * provided helper is_prime_big) and stores the qualifying primes in a dynamically
 * sized cache.  It can answer requests for any index n ≥ 1 (ignoring n = 0) in
 * any order; subsequent calls reuse the cached results.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
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

static seq_t *cache   = NULL;   /* cache of found terms */
static size_t cache_len   = 0;
static size_t cache_cap   = 0;

static uint64_t next_candidate = 3;   /* next odd integer to test for primality */

/* Grow the cache array to hold at least need elements */
static void grow_cache(size_t need)
{
    if (cache_cap >= need) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) return;              /* out‑of‑memory: abandon growth */
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* undefined for n < 1, safe guard */

    /* Generate terms until we have at least n entries */
    while (cache_len < (size_t)n) {
        /* Test candidate for primality */
        if (is_prime_big((seq_t)next_candidate)) {
            seq_t p = (seq_t)next_candidate;

            /* Check twin prime: p + 2 */
            seq_t p2 = p + (seq_t)2;
            if (!is_prime_big(p2)) {
                next_candidate += 2;
                continue;
            }

            /* Check quadratic condition: (p-1)^2 + 1 */
            seq_t d   = p - (seq_t)1;
            seq_t val = d * d + (seq_t)1;
            if (!is_prime_big(val)) {
                next_candidate += 2;
                continue;
            }

            /* Store qualifying prime */
            grow_cache(cache_len + 1);
            cache[cache_len++] = p;
        }
        next_candidate += 2;   /* move to next odd number */
    }

    return cache[(size_t)n - 1];
}
