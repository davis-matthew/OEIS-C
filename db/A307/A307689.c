/*
 * A307689  The number of rooted binary trees on n leaves with minimal Colless
 *          index.
 * https://oeis.org/A307689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 82, u32 n <= 164, u64 n <= 329,
 *            u128 n <= 658, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A307689: number of rooted binary trees on n leaves with minimal Colless index.
   Recurrence:
   a(1)=1.
   For n≥2:
      C[n] = min_{1≤a≤n/2} ( (n-2*a) + C[a] + C[n‑a] ).
      a[n] = Σ_{1≤a≤n/2, val==C[n]} ( a[a]*a[n‑a]   if a< n‑a
                                      binomial(a[a]+1,2)  if a==n‑a ).
   (The binomial term accounts for symmetry when the two subtrees have equal size.) */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>

static seq_t *a_cache = NULL;       /* a[n] values                           */
static idx_t *C_cache = NULL;       /* minimal Colless index for each n      */
static idx_t  cache_size = 0;       /* allocated capacity in indices         */
static idx_t  next_needed = 0;      /* next index not yet computed          */

/* Ensure a_cache[0..n] and C_cache[0..n] are computed. */
static void ensure_upto(idx_t n)
{
    if (n < next_needed) return;    /* already computed */

    /* Determine new capacity: at least n+1, double each time. */
    size_t new_cap = cache_size > 0 ? (size_t)cache_size : 2;
    while ((size_t)n + 1 > new_cap) new_cap <<= 1;

    /* Allocate or grow the arrays. */
    if (a_cache == NULL) {
        a_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        C_cache = (idx_t *)malloc(new_cap * sizeof(idx_t));
    } else {
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        C_cache = (idx_t *)realloc(C_cache, new_cap * sizeof(idx_t));
    }
    cache_size = (idx_t)new_cap;

    /* Initialise for the offset. */
    if (next_needed == 0) {
        a_cache[0] = 0;
        C_cache[0] = 0;
        if (cache_size > 1) {
            a_cache[1] = 1;
            C_cache[1] = 0;
        }
        next_needed = 2;
    }

    /* Compute values up to n. */
    for (idx_t i = next_needed; i <= n; ++i) {
        if (i == 1) continue;   /* already initialised */

        /* 1. Compute minimal Colless index C_cache[i] */
        idx_t best = UINT64_MAX;
        idx_t half = i >> 1;          /* floor(i/2)                        */
        for (idx_t a = 1; a <= half; ++a) {
            idx_t b    = i - a;          /* b >= a                           */
            idx_t diff = b - a;          /* non‑negative                     */
            idx_t val  = diff + C_cache[a] + C_cache[b];
            if (val < best) best = val;
        }
        C_cache[i] = best;

        /* 2. Count trees achieving the minimal index. */
        seq_t sum = 0;
        for (idx_t a = 1; a <= half; ++a) {
            idx_t b    = i - a;
            idx_t diff = b - a;
            idx_t val  = diff + C_cache[a] + C_cache[b];
            if (val != best) continue;   /* not minimal                    */

            if (a < b) {
                sum += a_cache[a] * a_cache[b];
            } else {
                /* a == b (only when i even) */
                seq_t m = a_cache[a];
                sum += (m * (m + (seq_t)1)) / (seq_t)2;  /* binomial(m+1,2) */
            }
        }
        a_cache[i] = sum;
    }
    next_needed = n + 1;
}

/* Public function: a(n) for n >= 1. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;
    ensure_upto(n);
    return a_cache[n];
}
