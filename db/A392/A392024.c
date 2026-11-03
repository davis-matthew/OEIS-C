/*
 * A392024  Positive integers k such that gcd(k, tau(k)) is prime.
 * https://oeis.org/A392024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Validated: partial, pass, 61 terms through n = 61 (2026-11-03)
 * Helpers:   gcd, is_prime_u128, is_prime_u64, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A392024: Positive integers k such that gcd(k, τ(k)) is prime */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include <limits.h>
#include "../utils/gcd.h"
#include "../utils/is_prime_u128.h"
#include "../utils/is_prime_u64.h"
#include "../utils/num_divisors.h"

/* Grow a dynamic array when needed.  The pointer is passed as (void **). */
static void ensure_capacity(size_t needed, size_t *cap, void **ptr, size_t elem_size)
{
    if (needed <= *cap) return;
    size_t new_cap = (*cap == 0) ? 4 : (*cap << 1);
    while (new_cap < needed) new_cap <<= 1;
    void *tmp = realloc(*ptr, new_cap * elem_size);
    if (!tmp) abort();               /* on OOM we abort as required */
    *ptr = tmp;
    *cap = new_cap;
}

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

/* Returns the nth term of the sequence (n >= 1) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* stored sequence values   */
    static size_t len   = 0;         /* number of stored terms */
    static size_t cap   = 0;         /* allocated capacity */
    static uint64_t next_k = 2;      /* next integer to test   */

    /* extend cache until it contains at least n terms */
    while (len < n)
    {
        uint64_t k = next_k++;
        uint64_t tau = num_divisors(k);
        seq_t g = gcd((seq_t)k, (seq_t)tau);
        if (isPrimeGeneric(g))
        {
            if (len + 1 > cap)
                ensure_capacity(len + 1, &cap, (void **)&cache, sizeof(seq_t));
            cache[len++] = (seq_t)k;
        }
    }
    return cache[n - 1];
}
