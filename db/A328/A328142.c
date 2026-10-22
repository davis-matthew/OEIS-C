/*
 * A328142  Elements of cycles for iterations of A329623: n -> |n - concat(sum
 *          of adjacent digits of n)|.
 * https://oeis.org/A328142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 23, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   compare_seq_t, seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A328142.
   The terms are the elements of all cycles of the map
   A329623(n) = | n – concat( sum of adjacent digits of n ) |.
   The cycle elements are the single digits 1…9 and, for each length L≥3
   and each d∈{1,…,7}, the number
        concat( d, 8‑d, d, 8‑d, … L times ) + 11.
   All such numbers are generated, sorted, and cached for fast access.
   --------------------------------------------------------------------- */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_from_str.h"   /* seq_from_str(char const *) → seq_t   */
#include "../utils/compare_seq_t.h"  /* compare_seq_t(void const *, void const *) */

static seq_t *cache = NULL;         /* array of computed sequence values      */
static size_t cache_len = 0;        /* number of elements stored              */
static idx_t  cache_max_n = 0;      /* largest n for which cache is ready     */

/* Ensure that cache contains at least `n` elements, sorted numerically. */
static void ensure_cache(idx_t n)
{
    if (n <= cache_max_n) return;               /* already prepared           */

    /* --------------------------------------------------------------------
       Compute how far we must generate the alternating numbers to reach n
       elements: 9 single‑digit fixed points + 7*(L-2) numbers of length L>=3.
    --------------------------------------------------------------------- */
    size_t need = (size_t)n;
    size_t Lmax  = 3;                            /* default minimal length   */
    if (need > 9) {
        /* 9 + 7*(Lmax-2) ≥ need  →  Lmax = ceil((need-9)/7)+2            */
        Lmax = ((need - 9 + 6) / 7) + 2;
    }
    size_t total = 9 + 7 * (Lmax - 2);           /* total numbers to store */

    /* Reallocate cache if larger than currently stored.                */
    seq_t *new_cache = (seq_t *)realloc(cache, total * sizeof(seq_t));
    if (!new_cache) {                           /* allocation failed         */
        exit(EXIT_FAILURE);
    }
    cache = new_cache;

    /* --------------------------------------------------------------------
       Generate the sequence entries (unsorted) and fill cache[0..total-1].
    --------------------------------------------------------------------- */
    size_t idx = 0;

    /* 1 … 9 : fixed points                                                      */
    for (seq_t v = 1; v <= 9; ++v) {
        cache[idx++] = v;
    }

    /* Numbers of the form alternating(d,8-d,… L times) + 11  (L≥3).            */
    for (size_t L = 3; L <= Lmax; ++L) {
        for (int d = 1; d <= 7; ++d) {
            /* Build the L‑digit alternating number as a decimal string.      */
            char buf[2000];                     /* sufficient for L≈1400      */
            size_t pos = 0;
            for (size_t i = 0; i < L; ++i) {
                char dig = (i & 1) ? ('0' + (8 - d)) : ('0' + d);
                buf[pos++] = dig;
            }
            buf[pos] = '\0';

            seq_t val = seq_from_str(buf);     /* convert string to seq_t      */
            val += 11;                         /* add 11 part of the rule      */
            cache[idx++] = val;                /* store in cache              */
        }
    }

    /* Cache should now contain 'total' elements.                              */
    cache_len  = total;
    cache_max_n = n;

    /* Sort the cache numerically once.                                         */
    qsort(cache, cache_len, sizeof(seq_t), compare_seq_t);
}

/* -------------------------------------------------------------------------
   Public function required by the platform.
   ------------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n = 0; safe guard          */
    ensure_cache(n);              /* guarantees cache up to n                */
    return cache[n - 1];          /* array is 0‑based, OEIS is 1‑based        */
}
