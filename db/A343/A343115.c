/*
 * A343115  Numbers k such that k^2 divides 5^k - 3^k - 2^k.
 * https://oeis.org/A343115
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 12, u32 n <= 14, u64 n <= 14, u128 n <= 14,
 *            big any n
 * Validated: partial, pass, 14 terms through n = 14 (2026-11-03)
 * Helpers:   mem, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: brute force search for k such that k^2 divides 5^k - 3^k - 2^k */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;     /* array of terms */
    static size_t  cache_size = 0;  /* number of stored terms */
    static size_t  cache_cap  = 0;  /* allocated capacity */
    static idx_t   checked_k  = 0;  /* last k tested */

    /* grow cache if necessary */
    if (cache_size < n) {
        /* ensure enough space for new terms */
        size_t required = n;
        if (cache_cap < required) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            while (new_cap < required) new_cap <<= 1;
            /* realloc with proper casting */
            void *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) exit(EXIT_FAILURE);
            cache = (seq_t *)tmp;
            cache_cap = new_cap;
        }

        /* test successive k values until we have n terms */
        while (cache_size < n) {
            ++checked_k;
            seq_t k = (seq_t)checked_k;
            seq_t m = k * k;                         /* modulus k^2 */

            seq_t a5 = powmod((seq_t)5, checked_k, m);
            seq_t a3 = powmod((seq_t)3, checked_k, m);
            seq_t a2 = powmod((seq_t)2, checked_k, m);

            seq_t d = (a5 + m - a3) % m;             /* (5^k - 3^k) mod m */
            d = (d + m - a2) % m;                   /* (5^k - 3^k - 2^k) mod m */

            if (d == 0) {                            /* k^2 | (5^k - 3^k - 2^k) */
                cache[cache_size++] = k;
            }
        }
    }

    return cache[n - 1];
}
