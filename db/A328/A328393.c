/*
 * A328393  Numbers whose arithmetic derivative (A003415) is a squarefree
 *          number (A005117).
 * https://oeis.org/A328393
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 142, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate integers and keep those whose arithmetic derivative
   (A003415) is squarefree (A005117).  a(1) is the first such integer.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_squarefree.h"

/* Compute the arithmetic derivative of a positive integer x.
   Implementation relies on prime factorisation of x (as uint64_t).
   It is adequate for all terms of A328393 up to n ≈ 10000, which
   comfortably fit into 64 bits. */
static seq_t derivative_of(seq_t x)
{
    uint64_t ux = (uint64_t)x;      /* x is assumed to fit in 64 bits */
    seq_t der = 0;
    uint64_t temp = ux;

    for (uint64_t p = 2; p * p <= temp; ++p) {
        if (temp % p == 0) {
            unsigned int e = 0;
            while (temp % p == 0) {
                temp /= p;
                ++e;
            }
            der += (seq_t)(ux / p) * (seq_t)e;
        }
    }

    if (temp > 1) {                /* remaining prime factor */
        der += (seq_t)(ux / temp);
    }

    return der;
}

/* Return the n-th term of A328393 (1‑based). */
seq_t a(idx_t n)
{
    /* Local cache of found terms */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static size_t len   = 0;   /* number of terms stored */

    /* Last candidate examined */
    static seq_t cur = 1;

    /* Extend cache until we have at least n terms */
    while (len < n) {
        ++cur;                       /* next integer to test */

        seq_t der = derivative_of(cur);
        if (is_squarefree(der)) {    /* derivative is squarefree */
            if (len == cap) {
                size_t newcap = (cap == 0) ? 8 : cap << 1;
                seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!tmp) abort();   /* unrecoverable failure */
                cache = tmp;
                cap = newcap;
            }
            cache[len++] = cur;
        }
    }

    return cache[n - 1];
}
