/*
 * A353337  Number of ways to write n as a product of the terms of A028260
 *          larger than 1; a(1) = 1 by convention (an empty product).
 * https://oeis.org/A353337
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9179, u16 n <= 9179, u32 n <= 9179, u64 n <= 9179,
 *            u128 n <= 9179, big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS formula: a(n) = number of factorizations of n into factors k > 1
    for which Ω(k) (prime factor multiplicity) is even. */

/* Standard library */
#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: smallest‑prime‑factor sieve */
#include "../utils/build_spf.h"

/* Global state for on‑the‑fly computation */
static idx_t  max_precomp = 0;      /* highest n that has been precomputed */
static idx_t *spf = NULL;           /* spf[0..n] smallest prime factor */
static unsigned char *parity = NULL;/* 0 = even Ω, 1 = odd Ω */
static seq_t *cache_val = NULL;     /* memoised a(n) */
static unsigned char *cache_flag = NULL; /* 1 = value stored */
static idx_t  cache_size = 0;       /* allocated array size */

/* Pre‑compute spf array and Ω parity up to n */
static void ensure_precomp(idx_t n)
{
    if (n <= max_precomp) return;

    /* free previous tables */
    free(spf);
    free(parity);

    /* build spf array */
    build_spf(n, &spf);
    if (!spf) abort();

    /* allocate parity table */
    parity = (unsigned char *)malloc((n + 1) * sizeof(unsigned char));
    if (!parity) abort();

    /* Ω(1) = 0 (even); compute parity iteratively */
    parity[1] = 0;
    for (idx_t i = 2; i <= n; ++i)
        parity[i] = parity[i / spf[i]] ^ 1;  /* toggle parity */

    max_precomp = n;
}

/* Ensure cache arrays can hold index n */
static void ensure_cache_size(idx_t n)
{
    if (n < cache_size) return;

    idx_t new_sz = n + 1;
    seq_t *new_val  = (seq_t *)realloc(cache_val,  new_sz * sizeof(seq_t));
    unsigned char *new_flg = (unsigned char *)realloc(cache_flag, new_sz * sizeof(unsigned char));

    if (!new_val || !new_flg) abort();

    /* new slots start as 'not computed' */
    for (idx_t i = cache_size; i < new_sz; ++i) new_flg[i] = 0;

    cache_val   = new_val;
    cache_flag  = new_flg;
    cache_size  = new_sz;
}

/* Recursive count of unordered factorizations of `rem` using
   factors >= min_factor, each having even Ω. */
static seq_t count_factorizations(idx_t rem, idx_t min_factor)
{
    if (rem == 1) return 1;              /* one way: product of chosen factors */

    seq_t total = 0;
    for (idx_t d = min_factor; d <= rem; ++d) {
        if ((rem % d == 0) && (parity[d] == 0))
            total += count_factorizations(rem / d, d);
    }
    return total;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 1) return 1;                /* empty product convention */

    ensure_precomp(n);
    ensure_cache_size(n);

    if (cache_flag[n]) return cache_val[n];

    seq_t result = count_factorizations(n, 2);
    cache_val[n]   = result;
    cache_flag[n]  = 1;
    return result;
}
