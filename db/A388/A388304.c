/*
 * A388304  Numbers that have several prime factors and all prime factors have
 *          the same exponent > 2.
 * https://oeis.org/A388304
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 none, u64 n <= 10000, u128 n <= 10000,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 9863 (2026-11-03)
 * Helpers:   factor_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A388304: Numbers that have several prime factors and all prime factors have the same exponent > 2.
   Computation:
   Build all numbers of the form (p1*p2*...*pk)^e with k≥2 distinct primes, e≥3,
   up to a fixed limit on the base k.  Then sort these numbers and return the nth term.  */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <stddef.h>
#include "../utils/factor_u64.h"
#include "../utils/mem.h"

/* Append a value to the cached array, expanding it if necessary.  `p` is a
   pointer to the array pointer. */
static void append_candidate(seq_t **p, size_t *len, size_t *cap, seq_t val)
{
    if (*len == *cap) {
        size_t newcap = (*cap == 0) ? 16 : (*cap << 1);
        seq_t *tmp = mem_resize(seq_t, *p, newcap);
        if (!tmp) {
            exit(EXIT_FAILURE);
        }
        *p = tmp;
        *cap = newcap;
    }
    (*p)[*len] = val;
    (*len)++;
}

/* comparison function for qsort */
static int cmp_seq_t(const void *p1, const void *p2)
{
    const seq_t *x = (const seq_t *)p1;
    const seq_t *y = (const seq_t *)p2;
    if (*x < *y) return -1;
    if (*y < *x) return 1;
    return 0;
}

static seq_t *cache = NULL;   /* sorted sequence values */
static size_t cache_len = 0;

/* Helper: check if n is composite square‑free (i.e. ω(n)=Ω(n)≥2) */
static bool is_sqfree_comp(uint64_t n)
{
    uint64_t pr[64], ex[64];
    size_t cnt = 0;
    factor_u64(n, pr, ex, &cnt);
    if (cnt < 2) return false;
    for (size_t i = 0; i < cnt; ++i)
        if (ex[i] != 1) return false;
    return true;
}

/* Build the sequence up to at least 'need' terms.  */
static void build_seq(idx_t need)
{
    if (cache_len >= need) return;

    /* Maximum base to explore.  Chosen so that k^3 for k≈20000 already yields >3×10^12,
       comfortably covering the first 10000 terms.  */
    const uint64_t MAX_K = 20000;
    seq_t *vals = NULL;
    size_t vals_cap = 0, vals_len = 0;

    for (uint64_t k = 2; k <= MAX_K; ++k) {
        if (!is_sqfree_comp(k)) continue;

        /* Start with exponent 3 */
        seq_t val = (seq_t) ( (unsigned __int128) k * k * k );
        append_candidate(&vals, &vals_len, &vals_cap, val);

        /* Continue with higher exponents until overflow */
        for (unsigned exp = 4; ; ++exp) {
            __uint128_t next = ( __uint128_t ) val * k;
            if (next > ( __uint128_t ) UINT64_MAX) break; /* too large for 64‑bit */
            val = (seq_t) next;
            append_candidate(&vals, &vals_len, &vals_cap, val);
        }
    }

    /* Sort the gathered values */
    mem_qsort(vals, vals_len, sizeof(seq_t), cmp_seq_t);

    cache = vals;
    cache_len = vals_len;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for n<1, but guard */
    build_seq(n);
    return cache[n-1];
}
