/*
 * A386075  Primes having only {0, 4, 7, 9} as digits.
 * https://oeis.org/A386075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth prime whose decimal representation uses only the
 *            digits {0,4,7,9}. Computed by generating all such numbers in
 *            increasing length order and testing for primality
 * Range:     u8 n <= 4, u16 n <= 58, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 7154 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
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

static seq_t *prime_cache = NULL;
static size_t cache_cap = 0;
static idx_t prime_cnt   = 0;
static idx_t next_len    = 1;  /* next number length to generate */

/* Ensure the cache has room for at least 'need' elements. */
static void ensure_cache_cap(idx_t need)
{
    if ((size_t)need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < (size_t)need) new_cap = need;
        seq_t *tmp = mem_resize(seq_t, prime_cache, new_cap);
        if (!tmp) exit(EXIT_FAILURE);
        prime_cache = tmp;
        cache_cap = new_cap;
    }
}

/* Recursively generate all numbers of 'len' digits formed by
   digits {0,4,7,9} (leading digit cannot be 0) and test them for
   primality.  Found primes are appended to the cache in order. */
static void gen_len_recursive(idx_t pos, seq_t acc, idx_t len)
{
    if (pos == len) {
        if (is_prime_big(acc)) {
            ensure_cache_cap(prime_cnt + 1);
            prime_cache[prime_cnt++] = acc;
        }
        return;
    }

    if (pos == 0) { /* first digit: 4,7,9 */
        static const uint8_t digits0[3] = {4,7,9};
        for (size_t i = 0; i < 3; ++i)
            gen_len_recursive(pos + 1, acc * 10 + digits0[i], len);
    } else {          /* following digits: 0,4,7,9 */
        static const uint8_t digits1[4] = {0,4,7,9};
        for (size_t i = 0; i < 4; ++i)
            gen_len_recursive(pos + 1, acc * 10 + digits1[i], len);
    }
}

/* Generate primes until we have at least 'target' terms. */
static void build_up_to(idx_t target)
{
    while (prime_cnt < target) {
        gen_len_recursive(0, (seq_t)0, next_len);
        next_len++;  /* all numbers of this length have been processed */
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    build_up_to(n);
    return prime_cache[n - 1];
}
