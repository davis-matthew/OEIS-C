/*
 * A244555  Primes which remain prime when the last digit is replaced by its
 *          square.
 * https://oeis.org/A244555
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 1790, u32 n <= 9000, u64 n <= 9000,
 *            u128 n <= 9000, big any n
 * Validated: standard, pass, 71 terms through n = 8638 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A244555: Primes remaining prime after replacing the last digit by its square.
   This implementation searches primes in increasing order and retains those that satisfy
   the property. */
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

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

/* Static cache for the generated sequence */
static seq_t *cache = NULL;      /* array of found primes            */
static size_t cache_size = 0;    /* number of stored terms           */
static size_t cache_cap   = 0;   /* allocated capacity of cache      */
static seq_t next_candidate = 2; /* next integer to test for primality */

/* Grow cache capacity to hold at least 'needed' entries */
static void grow_cache(size_t needed)
{
    if (needed <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 4;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
    if (!tmp)
        abort(); /* allocation failed */
    cache = tmp;
    cache_cap = new_cap;
}

/* Generate terms up to index 'n', storing them in the cache */
static void ensure_cache(idx_t n)
{
    if (cache_size >= (size_t)n)
        return; /* already have enough */

    while (cache_size < (size_t)n) {
        seq_t p = next_candidate; /* current candidate */

        /* only consider primes */
        if (isPrimeGeneric(p)) {
            /* compute q by replacing last digit of p with its square */
            seq_t base   = p / 10;
            seq_t d_seq = p % 10;
            int d = (int)((long long)d_seq);  /* last digit 0–9 */
            seq_t sq  = (seq_t)d * (seq_t)d;   /* d*d : 1–81 */
            int len   = (sq < 10) ? 1 : 2;     /* # of decimal digits of square */
            seq_t factor = pow10((idx_t)len);  /* 10^len              */
            seq_t q = base * factor + sq;     /* new number           */

            if (isPrimeGeneric(q)) {          /* test property        */
                grow_cache(cache_size + 1);
                cache[cache_size++] = p;      /* store prime           */
            }
        }

        ++next_candidate; /* next integer to test */
    }
}

/* Public function required by the build */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for 0          */
    ensure_cache(n);              /* make sure we have n terms */
    return cache[n - 1];           /* 1‑based indexing         */
}
