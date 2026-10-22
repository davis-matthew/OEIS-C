/*
 * A392564  Cubes of numbers that are neither squarefree nor squareful.
 * https://oeis.org/A392564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A332785(n)^3
 * Range:     u8 none, u16 n <= 6, u32 n <= 564, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed by brute‑force search */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

/* static cache of computed cubes */
static seq_t *cache = NULL;       /* dynamically resized array of cubes */
static size_t cache_size = 0;     /* allocated capacity (in elements) */
static size_t cache_used = 0;     /* number of cubes stored */

/* next integer candidate for checking */
static uint64_t candidate = 2;

/* Ensure the cache has room for at least one more element */
static void ensure_cache_capacity(void)
{
    if (cache_used >= cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(1);          /* out of memory */
        cache = tmp;
        cache_size = new_cap;
    }
}

/* Test whether m is neither squarefree nor squareful:
   it must have at least one prime exponent >= 2
   and at least one prime exponent = 1                      */
static bool qualifies(uint64_t m)
{
    bool has_ge2 = false;
    bool has_eq1 = false;
    uint64_t x = m;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            unsigned cnt = 0;
            while (x % p == 0) {
                x /= p;
                ++cnt;
            }
            if (cnt >= 2) has_ge2 = true;
            else if (cnt == 1) has_eq1 = true;
        }
    }
    if (x > 1) {          /* remaining prime factor with exponent 1 */
        has_eq1 = true;
    }
    return has_ge2 && has_eq1;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Build cache up to desired index */
    while ((idx_t)cache_used < n) {
        /* Find next qualifying integer */
        while (!qualifies(candidate))
            ++candidate;

        /* Compute its cube and store */
        seq_t base = (seq_t)candidate;
        seq_t cube = base * base * base;

        ensure_cache_capacity();
        cache[cache_used++] = cube;
        ++candidate; /* advance to next integer for subsequent searches */
    }

    return cache[n - 1];
}
