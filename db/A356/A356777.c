/*
 * A356777  G.f.: Sum_{n=-oo..+oo} x^(n^2) * C(x)^(2*n-1), where C(x) = 1 +
 *          x*C(x)^2 is a g.f. of the Catalan numbers (A000108).
 * https://oeis.org/A356777
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 38, i16 n <= 149, i32 n <= 587, i64 n <= 2234,
 *            i128 n <= 2555, big any n
 * Helpers:   binomial64, term_for_nk
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A356777:  a(m) = Σ_{n^2 ≤ m ≤ n^2+n} (-1)^(m-n^2)
 *  * C(2n-(m-n^2), m-n^2) * (2n+1)/(2n-2(m-n^2)+1)   (double‑sum extraction)
 *  The sequence is built by accumulating all pair (n,k) with m = n^2+k.
 *  For each new maximum index we recompute the whole table up to that index.
 *  128‑bit integer arithmetic is used only for intermediate products
 *  (supported by GCC/Clang); the final values are stored in seq_t.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/binomial64.h"
#include "../utils/term_for_nk.h"

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/*  cached table of sequence values                                    */
static seq_t *cache          = NULL;
static size_t cache_cap      = 0;   /* allocated size */
static idx_t  cache_max      = 0;   /* largest index computed      */

/* ------------------------------------------------------------------ */
seq_t
a(idx_t n)
{
    /* grow table if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        /* temporary 128‑bit accumulator */
        __int128 *tmp128 = (__int128 *)malloc(new_cap * sizeof(__int128));
        if (!tmp128) return (seq_t)0;          /* out of memory */
        for (size_t i = 0; i < new_cap; ++i) tmp128[i] = 0;

        idx_t max_n = (idx_t)sqrt((double)n);
        for (idx_t tn = 0; tn <= max_n; ++tn) {
            for (idx_t k = 0; k <= tn; ++k) {
                idx_t m = tn*tn + k;
                if (m > n) break;
                seq_t term = term_for_nk(tn, k);
                tmp128[m] += (__int128)term;
            }
        }

        /* convert 128‑bit accumulated values to seq_t */
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) { free(tmp128); return (seq_t)0; }
        for (size_t i = 0; i < new_cap; ++i) tmp[i] = (seq_t)tmp128[i];
        free(tmp128);

        /* dispose old cache and install new one */
        free(cache);
        cache = tmp;
        cache_cap = new_cap;
        cache_max = n;
    }

    return cache[n];
}
