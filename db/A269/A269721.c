/*
 * A269721  Integers k such that k, k+2, k+4 and k+6 are prime powers
 *          (A000961).
 * https://oeis.org/A269721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   bit_length, build_primes_upto, ipow_i32, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A269721 – integers k such that k, k+2, k+4, k+6 are prime powers
 * (A000961) and at least one of them is a power of 3. */
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>

/* Utility headers provided by the build system. */
#include "../utils/bit_length.h"
#include "../utils/build_primes_upto.h"
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

/* Static cache for the prime list and for the computed sequence. */
static uint64_t *prime_list   = NULL;
static size_t   prime_count   = 0;
static idx_t    prime_limit   = 0;

static seq_t  *seq_cache      = NULL;
static size_t  seq_len        = 0;
static size_t  seq_cap        = 0;
static idx_t   search_next    = 1;

/* Ensure that prime_list contains all primes ≤ limit.  rebuild each time
 * limit increases. */
static void ensure_prime_limit(idx_t limit)
{
    if (prime_limit >= limit) return;
    if (prime_list != NULL) free(prime_list);
    build_primes_upto(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Increase the capacity of seq_cache to at least need. */
static void ensure_seq_capacity(size_t need)
{
    if (seq_cap >= need) return;
    size_t new_cap = seq_cap ?: 8;
    while (new_cap < need) new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, seq_cache, new_cap);
    seq_cache = tmp;
    seq_cap = new_cap;
}

/* Test whether a 64‑bit number is a prime power. */
static bool is_prime_power_small(uint64_t m)
{
    if (m == 1) return true;
    uint64_t rem = m;
    for (size_t i = 0; i < prime_count; ++i) {
        uint64_t p = prime_list[i];
        if (p > rem / p) break;                /* p*p > rem */
        if (rem % p == 0) {
            while (rem % p == 0) rem /= p;
            if (rem > 1) return false;          /* found a second distinct prime factor */
            return true;                        /* only one prime factor */
        }
    }
    /* rem is 1 or a prime > sqrt(m) – both are prime powers. */
    return true;
}

/* Floor of integer e‑th root of a seq_t value (binary search). */
static seq_t root_floor(seq_t v, int e)
{
    if (e <= 1) return v;
    seq_t low = 1, high = 1;
    /* Find an upper bound for the root. */
    while (true) {
        seq_t high_pow = ipow_i32(high, e);
        if (high_pow > v) break;
        if (high >= ((seq_t)1 << 63)) break;   /* avoid overflow of high */
        high <<= 1;
    }
    /* Binary search on [low, high]. */
    while (low < high) {
        seq_t mid = low + (high - low + 1) / 2;
        seq_t mid_pow = ipow_i32(mid, e);
        if (mid_pow <= v)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Test whether a large (≥ 2^64) value is a prime power. */
static bool is_prime_power_large(seq_t v)
{
    if (is_prime_big(v)) return true;          /* v itself is prime */
    size_t max_e = bit_length(v);
    for (int e = 2; e <= (int)max_e; ++e) {
        seq_t r   = root_floor(v, e);
        seq_t pow = ipow_i32(r, e);
        if (pow == v && is_prime_big(r)) return true;
    }
    return false;
}

/* Wrapper selecting the appropriate routine. */
static bool is_prime_power(seq_t x)
{
    if (x <= (seq_t)UINT64_MAX)
        return is_prime_power_small((uint64_t)x);
    else
        return is_prime_power_large(x);
}

/* Test whether a value is a power of 3. */
static bool is_power_of_3(seq_t v)
{
    if (v == 0) return false;
    while (v % 3 == 0) v /= 3;
    return v == 1;
}

/* Main sequence routine. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n=0; return 0 */
    while (seq_len < n) {
        idx_t k = search_next;
        /* Ensure primes up to sqrt(k+6) are available. */
        idx_t limit = k + 6;
        ensure_prime_limit(limit);

        bool ok[4];
        bool has3 = false;
        for (int i = 0; i < 4; ++i) {
            seq_t val = (seq_t)k + (seq_t)(2 * i);
            ok[i] = is_prime_power(val);
            if (ok[i] && is_power_of_3(val)) has3 = true;
        }
        if (ok[0] && ok[1] && ok[2] && ok[3] && has3) {
            ensure_seq_capacity(seq_len + 1);
            seq_cache[seq_len++] = (seq_t)k;
        }
        ++search_next;
    }
    return seq_cache[n - 1];
}
