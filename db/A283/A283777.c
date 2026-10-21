/*
 * A283777  Numbers k such that floor(k*e) is even.
 * https://oeis.org/A283777
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth positive integer k such that ⌊k·e⌋ is even, where
 *            e = 2.718281828459045…
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const double _E = 2.71828182845904523536L;      /* high‑precision e */

/* ---------- cache for already computed terms ---------- */
static seq_t *k_cache = NULL;        /* cache of found k's           */
static size_t   k_len   = 0;         /* number of stored terms      */
static size_t   k_cap   = 0;         /* allocated capacity          */
static idx_t    next_k  = 1;         /* next integer to test        */

/* ensure at least 'need' capacity in the cache */
static void ensure_k_cap(size_t need) {
    if (need <= k_cap) return;
    size_t new_cap = k_cap ? k_cap * 2 : 64;
    while (new_cap < need) new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(k_cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                /* out of memory – fatal */
    k_cache = tmp;
    k_cap   = new_cap;
}

/* ----- main sequence function ----- */
seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined for n<1, but return 0 */
    if (n <= k_len) return k_cache[n - 1];  /* already computed */

    /* compute further terms until we have n of them */
    while (k_len < (size_t)n) {
        /* test consecutive integers k */
        const double prod = (double)next_k * _E;
        const idx_t floor_val = (idx_t)prod;         /* truncate = floor for k>0 */
        if ((floor_val & 1) == 0) {                  /* even */
            /* store this k */
            ensure_k_cap(k_len + 1);
            k_cache[k_len++] = (seq_t)next_k;
        }
        ++next_k;
    }
    return k_cache[n - 1];
}
