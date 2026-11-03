/*
 * A337491  Numbers k such that exactly one of 2*k + 3 and 4*k + 3 is prime.
 * https://oeis.org/A337491
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 4959, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Validated: standard, pass, 75 terms through n = 4960 (2026-11-03)
 * Helpers:   is_prime_u128, is_prime_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A337491: numbers k such that exactly one of 2*k+3 and 4*k+3 is prime */
/* Algorithm: sequentially test k values, cache results, and return the nth term. */

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

seq_t a(idx_t n)
{
    /* static cache holds the sequence values found so far */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t last_k = 0;  /* highest k already tested */

    if (n == 0) return 0;  /* safeguard for 0 index */

    /* return pre‑computed value when possible */
    if (len >= (size_t)n) return cache[n - 1];

    /* generate elements until we have at least n terms */
    while (len < (size_t)n)
    {
        last_k++;

        /* compute 2*k+3 and 4*k+3 as seq_t */
        seq_t v2 = (seq_t)last_k * (seq_t)2 + (seq_t)3;
        seq_t v4 = (seq_t)last_k * (seq_t)4 + (seq_t)3;

        bool p2 = isPrimeGeneric(v2);
        bool p4 = isPrimeGeneric(v4);

        /* store k if exactly one of the expressions is prime */
        if (p2 ^ p4)
        {
            if (len == cap)
            {
                size_t new_cap = cap ? cap << 1 : 64;
                seq_t *p = mem_resize(seq_t, cache, new_cap);
                cache = p;
                cap = new_cap;
            }
            cache[len++] = (seq_t)last_k;
        }
    }

    return cache[n - 1];
}
