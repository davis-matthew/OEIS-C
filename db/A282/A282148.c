/*
 * A282148  Numbers n with k digits in base x (MSD(n)=d_k, LSD(n)=d_1) such
 *          that, chosen one of their digits in position d_k < j < d_1, is
 *          Sum_{i=j..k}{(i-j+1)*d_i} = Sum_{i=1..j-1}{(j-i)*d_i}. Case x = 7.
 * https://oeis.org/A282148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 3645, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_fulcrum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A282148: numbers in base 7 that possess a “fulcrum” digit j
   such that
       Σ_{i=j..k} (i‑j+1)·d_i  =  Σ_{i=1..j‑1} (j‑i)·d_i
   where d_k is the most significant digit of the base‑7 representation. */
#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_fulcrum.h"

/* Forward declaration of the type‐specific helper functions that may be
   available from ../utils, but are not needed directly by this routine. */
#ifndef NO_UTILS
/* … any required utilities would be included here … */
#endif

/* static cache of previously found terms */
static seq_t *cache = NULL;        /* array of qualifying numbers          */
static size_t cache_len = 0;        /* how many terms are stored           */
static size_t cache_cap = 0;        /* current allocated capacity          */
static seq_t last_checked = 0;      /* last number that has been examined */

/* Append a new qualifying value to the cache, growing it if necessary. */
static void append_a(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 64 : cache_cap * 2;
#ifdef __cplusplus
        /* C++: use new[] and deep copy of existing elements */
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < cache_len; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
        cache = tmp;
#else
        /* C: use realloc, which simply byte‑copies the array */
        seq_t *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
#endif
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* --------------------------------------------------------------- */
/* Public entry point: a(idx_t n) returns the n‑th element (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;            /* undefined for n==0   */

    /* generate terms until we have at least n of them */
    while (cache_len < (size_t)n) {
        ++last_checked;
        if (is_fulcrum(last_checked))
            append_a(last_checked);
    }
    return cache[n - 1];
}
