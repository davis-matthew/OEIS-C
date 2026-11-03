/*
 * A337666  Numbers k such that any two parts of the k-th composition in
 *          standard order (A066099) have a common divisor > 1.
 * https://oeis.org/A337666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n‑th integer k whose k‑th composition in standard *
 *            order (A066099) has all parts > 1 and every pair of parts has a
 *            * common divisor > 1. The sequence has offset 1: a(1)=0, a(2)=2,
 *            …
 * Range:     u8 n <= 20, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mem.h"

/* --- Compute the parts of the k‑th composition (A066099) --- */
static size_t compose_parts(idx_t k, seq_t *parts)
{
    /* Record positions of 1‑bits in k (LSB = position 1). */
    seq_t pos[64];
    size_t m = 0;
    idx_t  tmp = k;
    size_t bitpos = 1;

    while (tmp) {
        if (tmp & 1ULL)
            pos[m++] = (seq_t)bitpos;
        tmp >>= 1;
        ++bitpos;
    }

    if (m == 0)                /* empty composition for k = 0 */
        return 0;

    /* First differences with a leading 0, then reverse. */
    seq_t diff[64];
    diff[0] = pos[0];
    for (size_t i = 1; i < m; ++i)
        diff[i] = pos[i] - pos[i-1];

    for (size_t i = 0; i < m; ++i)
        parts[i] = diff[m-1-i];

    return m;
}

/* --- Check whether the composition of k satisfies the pairwise gcd > 1 condition --- */
static bool ok_comp(idx_t k)
{
    seq_t parts[64];
    size_t nparts = compose_parts(k, parts);

    if (nparts == 0)           /* empty composition (k = 0) is allowed */
        return true;

    /* All parts must be > 1. */
    for (size_t i = 0; i < nparts; ++i)
        if (parts[i] == (seq_t)1)
            return false;

    /* Pairwise GCD test. */
    for (size_t i = 0; i < nparts; ++i) {
        for (size_t j = i + 1; j < nparts; ++j) {
            seq_t g = gcd(parts[i], parts[j]);
            if (g <= (seq_t)1)
                return false;
        }
    }
    return true;
}

/* --- Cache of qualifying k values --- */
static seq_t *cache        = NULL;
static size_t cache_len    = 0;
static size_t cache_cap    = 0;
static idx_t  next_k       = 0;

/* Ensure there is space for at least one more element. */
static void ensure_cache_space(void)
{
    if (cache_len >= cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp)
            exit(EXIT_FAILURE);   /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* ---------------------------------------------------------------------- */
/*  Public function required by the build system.                         */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)            /* offset 1 – a(1) is the first term */
        return (seq_t)0;

    size_t idx = (size_t)(n - (idx_t)1);   /* zero‑based index */

    while (cache_len <= idx) {          /* generate until we have enough */
        if (ok_comp(next_k)) {
            ensure_cache_space();
            cache[cache_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return cache[idx];
}
