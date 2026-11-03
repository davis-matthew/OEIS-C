/*
 * A371879  Prime numbers that yield a squarefree semiprime when any digit is
 *          removed.
 * https://oeis.org/A371879
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   factor_u64, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371879: prime numbers that yield a squarefree semiprime when any digit is removed */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper prototypes */
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

/* ---------- utilities ---------- */

/* Return true if x is a square‑free semiprime (product of two distinct primes). */
static bool squarefree_semiprime(uint64_t x)
{
    if (x < 4) return false;                /* smallest semiprime is 6 */
    uint64_t primes[32];
    uint64_t exps[32];
    size_t cnt = 0;
    factor_u64(x, primes, exps, &cnt);
    if (cnt != 2) return false;
    return exps[0] == 1 && exps[1] == 1;
}

/* Remove the i‑th digit (0 = most significant) from n and test the result. */
static bool satisfies_property(seq_t p)
{
    uint64_t n = (uint64_t)p;

    /* Count digits. */
    uint32_t dcnt = 1;
    uint64_t tmp = n;
    while (tmp >= 10) {
        tmp /= 10;
        ++dcnt;
    }

    /* Extract digits into array. */
    uint8_t digits[20];                  /* 20 > max digits of 64‑bit value */
    tmp = n;
    for (int i = (int)dcnt - 1; i >= 0; --i) {
        digits[i] = (uint8_t)(tmp % 10);
        tmp /= 10;
    }

    /* For each position, delete that digit and test the resulting number. */
    for (uint32_t i = 0; i < dcnt; ++i) {
        uint64_t val = 0;
        for (uint32_t j = 0; j < dcnt; ++j) {
            if (j == i) continue;
            val = val * 10 + digits[j];
        }
        if (!squarefree_semiprime(val))
            return false;
    }
    return true;
}

/* ---------- cache handling ---------- */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t cache_len  = 0;      /* number of cached terms */
static size_t cache_cap  = 0;      /* allocated capacity */
static seq_t  next_candidate;      /* next prime to test */
static bool   cache_initialized = false;

static void cache_grow(void)
{
    size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp) exit(1);                /* abort on allocation error */
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------------- the requested function ----------------- */
seq_t a(idx_t n)
{
    if (!cache_initialized) {
        cache_cap = 64;
        cache = mem_new(seq_t, cache_cap);
        if (!cache) exit(1);
        cache_len   = 0;
        cache_initialized = true;
        next_candidate = 557;         /* first known term – the offset */
    }

    /* Search forward until we have at least n terms cached. */
    while (cache_len < (size_t)n) {
        if (is_prime_big((seq_t)next_candidate) &&
            satisfies_property((seq_t)next_candidate)) {
            if (cache_len == cache_cap) cache_grow();
            cache[cache_len++] = next_candidate;
        }
        /* Step to the next odd number. */
        next_candidate += 2;
    }

    return cache[(size_t)(n - 1)];
}
