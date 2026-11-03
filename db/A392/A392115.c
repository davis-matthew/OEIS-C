/*
 * A392115  Numbers k such that 6*k - (greatest prime < 6*n) > (least prime >
 *          6*n) - 6*k.
 * https://oeis.org/A392115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = nth integer k such that * 12*k > (greatest prime < 6*k) +
 *            (least prime > 6*k)
 * Range:     u8 n <= 9, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Validated: partial, pass, 57 terms through n = 57 (2026-11-03)
 * Helpers:   is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* I.e. 6*k - gp > lp - 6*k where gp and lp are the nearest primes
 * to 6*k from below and above, respectively.                                   */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
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

/* Memoised cache of found terms. */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */
static size_t cache_cap = 0;   /* allocated capacity of cache array   */
static idx_t   last_k   = 0;   /* largest k already examined */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_cache_capacity(size_t need)
{
    if (cache_cap < need) {
        size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
        while (new_cap < need) new_cap *= 2;
        seq_t *new_cache = mem_resize(seq_t, cache, new_cap);
        cache = new_cache;
        cache_cap = new_cap;
    }
}

/* Compute the next k that satisfies the inequality.  The function
   updates last_k and appends the new term to cache. */
static void find_next_term(void)
{
    idx_t k = last_k + 1;              /* start searching just after last_k */

    for (;;) {
        /* 6*k */
        seq_t sixk = (seq_t)6 * (seq_t)k;

        /* greatest prime < 6*k */
        seq_t g = sixk - 1;
        while (!is_prime_big(g)) --g;

        /* least prime > 6*k */
        seq_t l = sixk + 1;
        while (!is_prime_big(l)) ++l;

        /* inequality: 12*k > g + l */
        seq_t left  = (seq_t)12 * (seq_t)k;
        seq_t right = g + l;
        if (left > right) {
            /* Found a valid k */
            if (cache_len == cache_cap) {
                ensure_cache_capacity(cache_len + 1);
            }
            cache[cache_len++] = (seq_t)k;
            last_k = k;
            break;
        }
        ++k;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1: a(1) is defined */

    /* Compute terms until we have the nth one. */
    while (cache_len < n) {
        find_next_term();
    }
    return cache[n - 1];
}
