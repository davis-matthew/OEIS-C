/*
 * A318785  Numbers which are prime if each digit is replaced by its 9's
 *          complement.
 * https://oeis.org/A318785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th natural number whose 9's complement is prime
 * Range:     u8 n <= 28, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Validated: partial, pass, 67 terms through n = 67 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <limits.h>
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/mem.h"

/* -----------------  Generic primality test ------------------------ */
static bool isPrimeGeneric(seq_t n)
{
    /* all seq_t are unsigned; use the smallest type that fits */
    if ((seq_t)(ULLONG_MAX) >= n) {
        return is_prime_u64((uint64_t)n);
    }
#if defined(__SIZEOF_INT128__)
    return is_prime_u128((__uint128_t)n);
#else
    /* fallback: if seq_t > 64‑bit but no 128‑bit support,
       order the test as simple trial division up to sqrt */
    seq_t limit = 0;
    for (seq_t i = 2; i*i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
#endif
}

/* Compute the 9's complement of an unsigned integer. */
static seq_t compl_of(idx_t x)
{
    uint8_t digs[256];
    size_t len = 0;
    idx_t tmp = x;

    do {
        uint8_t d = (uint8_t)(tmp % 10);
        digs[len++] = 9 - d;
        tmp /= 10;
    } while (tmp != 0);

    seq_t comp = 0;
    for (size_t i = 0; i < len; ++i) {
        /* rebuild the number from most significant digit to least. */
        comp = comp * 10 + digs[len - 1 - i];
    }
    return comp;
}

/* Static cache for previously computed terms. */
static seq_t *cache = NULL;   /* array of terms */
static size_t cache_cap = 0;  /* allocated capacity */
static size_t cache_len = 0;  /* number of terms stored */
static idx_t last_checked = 0;/* last integer tested for the property */

/* Grow the cache array to hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    /* realloc may fail; keep the old cache on failure. */
    if (tmp) {
        cache = tmp;
        cache_cap = new_cap;
    }
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;   /* undefined for offset 1; harmless */

    if (!cache) {
        cache = mem_new(seq_t, 16);
        cache_cap = 16;
        cache_len = 0;
        last_checked = 0;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        if (cache_len + 1 > cache_cap)
            ensure_cache_capacity(cache_len + 1);

        idx_t cand = last_checked + 1;
        seq_t comp = compl_of(cand);

        /* The 9's complement must be prime. */
        if (isPrimeGeneric(comp))
            cache[cache_len++] = (seq_t)cand;

        last_checked = cand;
    }

    return cache[n - 1];
}
