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
 * Helpers:   factor_u64, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A371879: prime numbers that yield a squarefree semiprime when any digit is removed */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Helper prototypes */
#include "../utils/is_prime_big.h"
#include "../utils/factor_u64.h"

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
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* abort on allocation error */
    cache = tmp;
    cache_cap = new_cap;
}

/* ----------------- the requested function ----------------- */
seq_t a(idx_t n)
{
    if (!cache_initialized) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
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
