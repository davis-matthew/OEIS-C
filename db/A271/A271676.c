/*
 * A271676  Prime powers k such that 3k + 4 is a perfect square.
 * https://oeis.org/A271676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   a(n) = prime power k such that 3*k + 4 is a perfect square. The
 *            function enumerates u with 3*k+4 = u^2, computes k, and keeps a
 *            static cache for memoization
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 4, u128 n <= 4,
 *            big any n
 * Validated: full, pass, 4 terms through n = 4 (2026-11-03)
 * Helpers:   bit_length, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/bit_length.h"
#include "../utils/ipow_i32.h"
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

/* ----------- Cache management ------------------------------------------ */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

static void cache_grow(size_t needed)
{
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 32;
    if (new_cap < needed) new_cap = needed;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) abort();
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------- Helper: integer r‑th root --------------------------------- */
static seq_t integer_root(seq_t x, idx_t r)
{
    if (r == 1) return x;
    seq_t low = 1, high = x;
    while (low <= high) {
        seq_t mid = (low + high) / 2;
        seq_t midpow = ipow_i32(mid, (int)r);
        if (midpow == x) return mid;
        if (midpow < x) {
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return 0;   /* not an integer root */
}

/* ----------- Helper: test prime–power --------------------------------- */
static bool is_prime_power(seq_t k)
{
    if (k <= 1) return false;
    if (is_prime_big(k)) return true;      /* k itself is prime */
    idx_t maxExp = bit_length(k);        /* upper bound for exponent */
    for (idx_t e = 2; e <= maxExp; ++e) {
        seq_t root = integer_root(k, e);
        if (!root) continue;
        if (ipow_i32(root, (int)e) == k && is_prime_big(root))
            return true;
    }
    return false;
}

/* ----------- Main sequence function ----------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1; guard against bad input */
    /* Persist the next u to test across calls */
    static idx_t last_u = 2;              /* start with u = 3 on first iteration */
    while (cache_len < n) {
        last_u++;                          /* next candidate u */
        seq_t u_sq = (seq_t)last_u * (seq_t)last_u;
        seq_t diff = u_sq - 4;
        if (diff % 3 == 0) {
            seq_t k = diff / 3;
            if (is_prime_power(k)) {
                cache_grow(cache_len + 1);
                cache[cache_len++] = k;
            }
        }
    }
    return cache[n - 1];
}
