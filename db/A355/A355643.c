/*
 * A355643  Numbers k having a divisor d such that d+k/d is prime.
 * https://oeis.org/A355643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2971, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9834 (2026-11-03)
 * Helpers:   ipow_i32, is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A355643: numbers k having a divisor d such that d + k/d is prime,
   in ascending order.  The function returns the n‑th term (1‑based). */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/ipow_i32.h"
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

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* cache stores the sequence terms computed so far */
static seq_t *cache = NULL;          /* array of terms           */
static size_t cache_size = 0;        /* number of terms stored   */
static size_t cache_cap  = 0;        /* current capacity         */
/* next integer to test, starting from 1 */
static idx_t next_candidate = 1;

/* Append a new term to the cache, reallocating if necessary.
   The cast to (seq_t *) is required for C++ where seq_t may be a class. */
static inline void add_term(seq_t value)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = value;
}

/* Generate terms until at least n terms are stored in the cache. */
static void generate_up_to(idx_t n)
{
    while (cache_size < n) {
        seq_t k = (seq_t)next_candidate;
        bool found = false;
        idx_t sqrt_k = (idx_t)floor_sqrt(k);
        for (idx_t d = 1; d <= sqrt_k; ++d) {
            if ((k % (seq_t)d) == 0) {
                seq_t e = k / (seq_t)d;
                seq_t sum = (seq_t)d + e;
                if (isPrimeGeneric(sum)) {
                    add_term(k);
                    found = true;
                    break;
                }
            }
        }
        ++next_candidate;
        (void)found; /* value used only for side‑effects */
    }
}

/* Return the n‑th term (1‑based index) of A355643.  The function works for
   any order of calls; it memoises results in a static cache. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, placeholder */
    generate_up_to(n);
    return cache[n - 1];
}
