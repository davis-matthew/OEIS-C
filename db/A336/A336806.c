/*
 * A336806  a(n) = prime(k) for the first k such that Product_{j=k..k+n-1}
 *          prime(j) mod Sum_{j=k..k+n-1} prime(j) is prime.
 * https://oeis.org/A336806
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(k) where k is the smallest index such that
 *            (Prod_{j=k}^{k+n-1} prime(j)) mod (Sum_{j=k}^{k+n-1} prime(j))
 *            is prime
 * Range:     u8 n <= 8, u16 n <= 168, u32 n <= 1382, u64 n <= 7700,
 *            u128 n <= 7700, big any n
 * Validated: standard, pass, 96 terms through n = 7487 (2026-11-03)
 * Helpers:   is_prime_u64, mem, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <string.h>
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

/* Static storage for prime list, its prefix sums and cache of results. */
static uint64_t *prime_list = NULL;
static size_t prime_cnt  = 0, prime_cap  = 0;
static seq_t  *prime_pref = NULL;   /* prime_pref[i] = sum of first i primes (prime_pref[0] = 0) */
static size_t pref_cap = 0;

static seq_t *cache   = NULL;
static size_t cache_cap = 0;

/* Ensure the prime list and its prefix sums contain at least 'need' primes. */
static void ensure_primes(size_t need)
{
    if (prime_cnt >= need) return;

    /* grow capacity */
    size_t new_cap = prime_cap == 0 ? 256 : prime_cap;
    while (new_cap < need) new_cap <<= 1;

    uint64_t *new_list = (uint64_t *)realloc(prime_list, new_cap * sizeof(uint64_t));
    if (!new_list) return;          /* allocation failure – unlikely in contest use */
    prime_list = new_list;

    seq_t *new_pref = mem_resize(seq_t, prime_pref, (new_cap + 1));
    if (!new_pref) return;
    prime_pref = new_pref;

    /* initialize prefix[0] if this is the first allocation */
    if (prime_cap == 0) prime_pref[0] = 0;

    prime_cap = new_cap;
    pref_cap  = new_cap + 1;

    /* generate missing primes */
    while (prime_cnt < need) {
        uint64_t nextp;
        if (prime_cnt == 0) nextp = 2;
        else nextp = next_prime(prime_list[prime_cnt - 1]);

        prime_list[prime_cnt] = nextp;
        prime_pref[prime_cnt + 1] = prime_pref[prime_cnt] + (seq_t)nextp;
        ++prime_cnt;
    }
}

/* Ensure the cache array can store a(n). */
static void ensure_cache(size_t n)
{
    if (n < cache_cap) return;
    size_t new_cap = cache_cap == 0 ? 8 : cache_cap;
    while (new_cap <= n) new_cap <<= 1;
    seq_t *new_arr = mem_new(seq_t, new_cap);
    if (!new_arr) return;
    if (cache) {
        mem_copy(new_arr, cache, cache_cap * sizeof(seq_t));
        mem_free(cache);
    }
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n < 2) return 0;   /* sequence is undefined for n < 2, offset 2 */

    ensure_cache((size_t)n);
    if (cache[n] != 0) return cache[n];

    seq_t answer = 0;
    /* Search for the smallest k. */
    for (idx_t k = 1;; ++k) {
        ensure_primes((size_t)(k + n - 1));

        seq_t sum = prime_pref[k + n - 1] - prime_pref[k - 1];
        seq_t prod = 1;
        for (idx_t i = 0; i < n; ++i) {
            prod = (prod * (seq_t)prime_list[k - 1 + i]) % sum;
        }
        if (is_prime_big(prod)) {
            answer = (seq_t)prime_list[k - 1];
            cache[n] = answer;
            return answer;
        }
    }
}
