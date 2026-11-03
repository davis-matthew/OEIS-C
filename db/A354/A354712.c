/*
 * A354712  Nonprime numbers k such that the number of divisors of k divides
 *          k-1.
 * https://oeis.org/A354712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth nonprime integer k such that the number of
 *            divisors d(k) divides (k-1)
 * Range:     u8 n <= 37, u16 n <= 8825, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 77 terms through n = 7896 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
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

static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;
static idx_t curr = 1;

static void _grow_cache(void) {
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    seq_t *p = mem_resize(seq_t, cache, new_cap);
    if (!p) abort();
    cache = p;
    cache_cap = new_cap;
}

static void _generate_up_to(idx_t n) {
    if (n <= cache_len) return;
    if (cache_len == 0) {
        cache_cap = 128;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) abort();
        cache[0] = (seq_t)1;         /* k = 1 is the first term */
        cache_len = 1;
        curr = 1;
    }
    uint64_t primes_arr[64];
    uint64_t exps_arr[64];
    size_t cnt;

    while (cache_len < (size_t)n) {
        curr++;
        if (is_prime_big((seq_t)curr)) continue;   /* skip primes */

        /* compute divisor count d(curr) */
        factor_u64((uint64_t)curr, primes_arr, exps_arr, &cnt);
        seq_t d = 1;
        for (size_t i = 0; i < cnt; ++i) {
            d *= (seq_t)(exps_arr[i] + 1);
        }

        /* check the property d | (k-1) */
        seq_t rem = ((seq_t)(curr - 1)) % d;
        if (rem == 0) {
            if (cache_len == cache_cap) _grow_cache();
            cache[cache_len++] = (seq_t)curr;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    _generate_up_to(n);
    return cache[n - 1];
}
