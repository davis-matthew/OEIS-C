/*
 * A364868  Numbers k such that 4*k+1 is the norm of a Gaussian prime.
 * https://oeis.org/A364868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (A055025(n+1) - 1)/4 Sequence A364868: numbers k such
 *            that 4*k+1 is the norm of a Gaussian prime (i.e., 4*k+1 is a
 *            prime ≡1 (mod 4) or the square of a prime ≡3 (mod 4))
 * Range:     u8 n <= 10, u16 n <= 64, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt, isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/isPrimeTrial.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* static cache for previously found k's */
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t size  = 0;     /* number of cached terms */
    static idx_t  next_k = 1;    /* next k to test (1‑based) */

    /* Grow cache until it contains at least n terms. */
    while ((size_t)n > size) {
        seq_t m = (seq_t)next_k * 4 + 1;   /* 4*k+1 */

        bool ok = false;

        /* 1. is m a prime? */
        if (isPrimeTrial(m)) {
            ok = true;
        } else {
            /* 2. m might be a square of a prime ≡3 mod 4 */
            seq_t r = floor_sqrt(m);           /* floor(sqrt(m)) */
            if (r * r == m) {                  /* exact square */
                if (isPrimeTrial(r) && ((r & 3) == 3)) {
                    ok = true;
                }
            }
        }

        if (ok) {
            if (size == cap) {                 /* extend the array */
                cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
                if (!tmp) abort();             /* allocation failed */
                cache = tmp;
            }
            cache[size++] = (seq_t)next_k;      /* store the qualifying k */
        }

        ++next_k;  /* examine the next potential k */
    }

    return cache[(size_t)n - 1];   /* 1‑based indexing */
}
