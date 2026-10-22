/*
 * A353427  Numbers k such that the k-th composition in standard order has all
 *          run-lengths > 1.
 * https://oeis.org/A353427
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 24, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A353427
   Numbers k such that the k‑th composition in standard order
   has all run‑lengths > 1.  There is no simple closed formula, so
   we generate the terms by a linear scan and cache the results. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* For C++ builds we use std::vector for convenience; C builds use
   explicit realloc. */
#ifdef __cplusplus
#include <vector>
#endif

/* ------------------------------------------------------------------ */
/* Helper: does the k‑th composition (in standard order) contain only
   run‑lengths larger than 1?  k is 0‑based.                          */
static bool is_valid_k(idx_t k)
{
    /* Collect the positions of 1‑bits in the reversed binary
       representation of k, together with a prepended 0.  The
       resulting differences are the parts before the final reversal. */
    idx_t parts[70];       /* 70 > max popcount of any 64‑bit number */
    int   len = 0;
    idx_t prev = 0;
    idx_t bits = k;
    int   bit_index = 0;

    while (bits != 0) {
        if (bits & 1U) {
            idx_t pos = (idx_t)(bit_index + 1); /* 1‑based position  */
            parts[len++] = pos - prev;
            prev = pos;
        }
        bits >>= 1;
        ++bit_index;
    }

    /* Empty composition is valid. */
    if (len == 0)
        return true;

    /* Scan the composition in reverse order to test run‑lengths. */
    bool ok = true;
    int  run = 1;   /* run length of the current part */
    for (int i = len - 1; i > 0; --i) {
        if (parts[i] == parts[i - 1]) {
            ++run;
        } else {
            if (run == 1) {        /* run of length 1 found */
                ok = false;
                break;
            }
            run = 1;
        }
    }
    if (ok && run == 1)  /* last run also has length 1 */
        ok = false;

    return ok;
}

/* ------------------------------------------------------------------ */
/* Cache of already found terms.  We store the successive values of
   k that satisfy the property.  The cache can grow on demand. */

#ifndef __cplusplus
/* C implementation */
static seq_t *valid_cache   = NULL;
static size_t cache_cap     = 0;       /* allocated capacity          */
static size_t cache_size    = 0;       /* number of terms currently in cache */
static idx_t  next_k        = 0;       /* next k to test                */

static void generate_until(idx_t target)
{
    while (cache_size < target) {
        if (is_valid_k(next_k)) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *tmp = (seq_t *)realloc(valid_cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();
                valid_cache = tmp;
                cache_cap = new_cap;
            }
            valid_cache[cache_size++] = (seq_t)next_k;
        }
        ++next_k;
    }
}
#else
/* C++ implementation */
static std::vector<seq_t> cache_v;
static idx_t next_k = 0;

static void generate_until(idx_t target)
{
    cache_v.reserve(target);
    while (cache_v.size() < target) {
        if (is_valid_k(next_k)) {
            cache_v.push_back((seq_t)next_k);
        }
        ++next_k;
    }
}
#endif

/* ------------------------------------------------------------------ */
/* Public sequence function.  n is 1‑based (a(1) = 0). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for n < 1, not expected */
    generate_until(n);
#ifdef __cplusplus
    return cache_v[n - 1];
#else
    return valid_cache[n - 1];
#endif
}
