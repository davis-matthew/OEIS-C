/*
 * A265048  Poincaré series for hyperbolic reflection group with Coxeter
 *          diagram o-(5)-o---o-(5)-o.
 * https://oeis.org/A265048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 22, u32 n <= 49, u64 n <= 104, u128 n <= 214,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A265048
 *
 *  Initial terms (a(0) … a(13)):
 *      1, 4, 9, 17, 30, 50, 80, 125, 193, 296, 450, 680, 1025, 1541
 *
 *  For n ≥ 14 the sequence satisfies the linear recurrence
 *
 *      a(n) =  2·a(n‑1) – a(n‑2) + a(n‑4) – a(n‑5)
 *            + a(n‑6) – a(n‑7) + a(n‑8) – a(n‑9)
 *            + a(n‑11) – 2·a(n‑12) + a(n‑13)
 *
 *  The function below memoises computed values so that calls may come
 *  in any order.  It works for all supported seq_t types.
 */

#include <stdlib.h>

/* External integer types supplied by the build */
extern struct seq_tag seq_tag;   /* placeholder for compatibility */

/* Static storage for memoisation */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* First 14 values of the sequence */
static const seq_t init_vals[14] = {
    1, 4, 9, 17, 30, 50, 80, 125, 193, 296,
    450, 680, 1025, 1541
};

/* Ensure that cache has entries 0 … n inclusive */
static void ensure_capacity(idx_t n)
{
    if (n < cache_size) return;

    idx_t old_size = cache_size;           /* size before growing */
    idx_t new_size = n + 1;                /* required size */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                     /* allocation failure */
    cache = tmp;
    cache_size = new_size;

    /* initialise missing initial terms (indices < 14) */
    if (old_size < 14) {
        for (idx_t i = old_size; i < 14 && i < cache_size; ++i)
            cache[i] = init_vals[i];
        old_size = 14;
    }

    /* compute new terms using the recurrence */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i - 1];   /* 2·a(i‑1)           */
        val -= cache[i - 2];                  /* – a(i‑2)            */
        val += cache[i - 4];                  /* + a(i‑4)            */
        val -= cache[i - 5];                  /* – a(i‑5)            */
        val += cache[i - 6];                  /* + a(i‑6)            */
        val -= cache[i - 7];                  /* – a(i‑7)            */
        val += cache[i - 8];                  /* + a(i‑8)            */
        val -= cache[i - 9];                  /* – a(i‑9)            */
        val += cache[i - 11];                 /* + a(i‑11)           */
        val -= cache[i - 12];                 /* – a(i‑12)           */
        val -= cache[i - 12];                 /* – a(i‑12) again     */
        val += cache[i - 13];                 /* + a(i‑13)           */
        cache[i] = val;
    }
}

/* Main function requested by the problem */
seq_t a(idx_t n)
{
    ensure_capacity(n);
    return cache[n];
}
