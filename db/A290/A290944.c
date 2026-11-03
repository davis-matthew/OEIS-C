/*
 * A290944  Primes p such that sum of digits of p^3 is a perfect square.
 * https://oeis.org/A290944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 1, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 60 terms through n = 8256 (2026-11-01)
 * Helpers:   digit_sum, ipow, ipow_i32, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: iterate primes and collect those where the sum of digits of p^3 is a perfect square */

#include <stddef.h>
#include <stdbool.h>

#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_sum.h"
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"
#include "../utils/next_prime.h"

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

/* Helper allocation for seq_t (arbitrary‑precision or builtin integer) */
static seq_t* seq_alloc(size_t cap) {
#ifdef __cplusplus
    return new seq_t[cap];
#else
    return (seq_t*)malloc(cap * sizeof(seq_t));
#endif
}

static seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) {
#ifdef __cplusplus
    seq_t* new_ptr = new seq_t[new_cap];
    for (size_t i = 0; i < old_cap; ++i)
        new_ptr[i] = old[i];
    delete[] old;
    return new_ptr;
#else
    return (seq_t*)realloc(old, new_cap * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    static seq_t *cache  = NULL;      /* array of found primes */
    static size_t  cap   = 0;          /* allocated capacity */
    static size_t  len   = 0;          /* number of primes stored */
    static uint64_t last_checked = 0;  /* last prime examined */

    /* expand cache until we have at least n primes */
    while (len < n) {
        /* get next prime */
        uint64_t prime = next_prime(last_checked);
        last_checked = prime;

        /* compute p^3 and sum of its digits */
        seq_t cube = ipow((seq_t)prime, (seq_t)3);
        seq_t sum  = digit_sum(cube);

        /* check whether sum is a perfect square */
        seq_t root = floor_sqrt(sum);
        if (root * root == sum) {
            /* ensure capacity */
            if (len == cap) {
                size_t new_cap = (cap == 0) ? 64 : cap << 1;
                seq_t *tmp = seq_realloc(cache, cap, new_cap);
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)prime;
        }
    }
    /* 1‑based indexing */
    return cache[n - 1];
}
