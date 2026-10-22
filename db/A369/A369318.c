/*
 * A369318  Numbers k such that A091255(k, k + 1) <> 1.
 * https://oeis.org/A369318
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 51, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes sequence A369318: numbers k such that the polynomial
   represented by the binary expansion of k and of k+1 are not coprime
   over GF(2).  The function returns the n-th term (offset 1). */

#include <stddef.h>     /* size_t */
#include <stdint.h>     /* uint64_t, etc. */
#include <stdlib.h>     /* malloc, realloc, free */
#include <stdbool.h>    /* bool */
#include "../utils/bitlen_seq_t.h"   /* bitlen_seq_t(seq_t) */

/* Compute the gcd of two polynomials over GF(2).  The polynomials are
   represented by the binary expansion of the integers a and b.           */
static inline seq_t poly_gcd(seq_t a, seq_t b)
{
    while (b != 0) {
        unsigned deg_b = bitlen_seq_t(b) - 1;
        /* reduce a by b repeatedly */
        seq_t r = a;
        while (r != 0) {
            unsigned deg_r = bitlen_seq_t(r) - 1;
            if (deg_r < deg_b)
                break;
            r ^= b << (deg_r - deg_b);
        }
        a = b;
        b = r;
    }
    return a;
}

/* Return true iff a pair (k, k+1) of polynomials over GF(2) is not coprime. */
static inline bool ok_k(seq_t k)
{
    return poly_gcd(k, k + 1) != (seq_t)1;
}

/* Main sequence function.  It lazily builds the sequence up to the
   requested index, caching the values for future calls. */
seq_t a(idx_t n)
{
    /* Cache of found terms. */
    static seq_t *cache      = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap  = 0;
    static idx_t  last_k     = 0;   /* last integer tested */

    if (n == 0)
        return 0;    /* Undefined for n < 1, but avoid UB */

    /* Grow cache until it contains at least n terms. */
    while (cache_size < n) {
        if (++last_k == 0)          /* overflow guard, not expected */
            break;

        if (ok_k(last_k)) {
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) {         /* allocation failure */
                    free(cache);
                    cache = NULL;
                    cache_cap = cache_size = 0;
                    return 0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = last_k;
        }
    }

    return cache[n - 1];
}
