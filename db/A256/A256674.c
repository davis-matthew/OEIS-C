/*
 * A256674  Numbers n such that n^2 + n + 712329866165608771 is prime.
 * https://oeis.org/A256674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 none, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A256674:  Numbers n such that n^2 + n + 712329866165608771 is prime.
   The function returns the n-th such integer (a(1) is the first). */

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

/* Constant from the definition of the sequence. */
static const seq_t CONST_VAL = (seq_t)712329866165608771ULL;

/* Static cache for already found terms. */
static seq_t *cache      = NULL;   /* array of found terms */
static size_t cache_len  = 0;      /* number of terms stored   */
static size_t cache_cap  = 0;      /* current capacity         */
static idx_t  last_checked = 0;    /* largest n already tested */

/* Ensure the cache can hold at least new_cap elements. */
static void grow_cache(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap << 1 : 8;
    while (cap < new_cap) cap <<= 1;
    seq_t *new_ptr = mem_new(seq_t, cap);
    if (!new_ptr) exit(1);          /* allocation failure */
    if (cache) {
        for (size_t i = 0; i < cache_len; ++i) new_ptr[i] = cache[i];
        mem_free(cache);
    }
    cache = new_ptr;
    cache_cap = cap;
}

/* Main function required by the problem statement. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined for n=0, return 0 */

    /* Search until we have found the n-th term. */
    while (cache_len < n) {
        ++last_checked;
        seq_t val = (seq_t)last_checked * last_checked +
                    (seq_t)last_checked + CONST_VAL;
        if (isPrimeGeneric(val)) {
            grow_cache(cache_len + 1);
            cache[cache_len++] = last_checked;
        }
    }

    return cache[n - 1];
}
