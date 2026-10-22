/*
 * A378918  Indices of octagonal pyramidal numbers that are both octagonal and
 *          octagonal pyramidal.
 * https://oeis.org/A378918
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A378918: Indices of octagonal pyramidal numbers that are also octagonal.
   The n-th term a(n) is the n-th non‑negative integer k such that the k-th
   octagonal pyramidal number
       P(k) = k(k+1)(2k-1)/2
   equals an octagonal number
       O(j) = j(3j-2)
   for some integer j≥0.
   The function a(idx_t n) returns a(n) for n≥1 (offset 1). */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Dynamic cache of already found indices */
static seq_t *a_cache = NULL;        /* array of indices (seq_t) */
static size_t a_cache_len = 0;       /* number of terms stored */
static size_t a_cache_cap = 0;       /* capacity of a_cache */
static idx_t next_test = 0;          /* next integer k to test     */

/* Ensure the cache has at least `need` capacity */
static void grow_cache(void)
{
    if (a_cache_cap == 0) {
        a_cache_cap = 16;
        a_cache = (seq_t *)malloc(a_cache_cap * sizeof(seq_t));
    } else {
        a_cache_cap <<= 1;
        a_cache = (seq_t *)realloc(a_cache, a_cache_cap * sizeof(seq_t));
    }
}

/* Append a new term to the cache */
static void append_term(seq_t val)
{
    if (a_cache_len == a_cache_cap)
        grow_cache();
    a_cache[a_cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Indexing: a(1) corresponds to the first term stored in a_cache[0] */
    if (n == 0) {
        /* Undefined for offset 1; return 0 as a safe fallback */
        return (seq_t)0;
    }

    /* Grow cache until we have at least n terms */
    while (a_cache_len < (size_t)n) {
        /* Compute D = 4 + 6 * k(k+1)(2k-1) for k = next_test */
        idx_t k = next_test;

        /* Use __int128 to detect overflow for 64‑bit seq_t */
        unsigned __int128 D_big = (unsigned __int128)4;
        if (sizeof(seq_t) <= 8) {               /* 64‑bit or smaller concrete type */
            unsigned __int128 term = (unsigned __int128)k;
            term *= (unsigned __int128)(k + 1);
            term *= (unsigned __int128)(2 * (unsigned __int128)k - 1);
            term *= (unsigned __int128)6;
            D_big += term;
            if (D_big > (unsigned __int128)(~(uint64_t)0)) {
                /* Value cannot be represented exactly in seq_t;
                   stop generating further terms for this build. */
                break;
            }
        }

        /* Compute in seq_t domain (big integer or 64‑bit) */
        seq_t D = (seq_t)4;
        seq_t t = (seq_t)k;
        t = t * (seq_t)(k + 1);
        /* Avoid overflow in 2*k-1 by computing in seq_t domain */
        seq_t two_k_minus_1 = (seq_t)2 * (seq_t)k - (seq_t)1;
        t = t * two_k_minus_1;
        t = t * (seq_t)6;
        D += t;

        /* Check perfect square */
        seq_t m = floor_sqrt(D);
        if (m * m == D) {
            if (k == 0 || (m % (seq_t)6) == (seq_t)4) {
                append_term((seq_t)k);
            }
        }

        next_test++;   /* advance to next k */
    }

    /* If n exceeds precomputed terms, nothing can be returned reliably;
       caller should avoid such requests for 64‑bit builds where the value
       would overflow.  Return 0 as a safe sentinel. */
    if (n > a_cache_len)
        return (seq_t)0;

    return a_cache[n - 1];
}
