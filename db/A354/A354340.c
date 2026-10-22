/*
 * A354340  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} d^(k/d + 1) )/(k * (n-k)!).
 * https://oeis.org/A354340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * Σ_{k=1..n} Σ_{t=1}^{⌊n/k⌋} k^t / ( t·(n−k·t)! )
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
/* use helper routines for allocating/rehashing seq_t arrays */
#include "../utils/seq_alloc.h"   /* seq_t* seq_alloc(size_t cap) */
#include "../utils/seq_realloc.h" /* seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap) */

static seq_t *fact_cache = NULL;      /* factorial cache: fact_cache[i] = i!  */
static size_t fact_cap  = 0;          /* current allocated size of fact_cache */

/* compute a(n) using the nested‑sum formula */
seq_t a(idx_t n)
{
    /* ensure factorial table up to n */
    if (fact_cap <= n) {
        size_t old_cap = fact_cap;
        size_t new_cap = n + 1;
        if (!fact_cache)
            fact_cache = seq_alloc(new_cap);
        else
            fact_cache = seq_realloc(fact_cache, old_cap, new_cap);
        fact_cap = new_cap;

        /* compute factorials from old_cap to n */
        for (idx_t i = old_cap; i <= n; ++i) {
            if (i == 0)
                fact_cache[0] = (seq_t)1;
            else
                fact_cache[i] = fact_cache[i - 1] * (seq_t)i;
        }
    }

    seq_t fact_n = fact_cache[n];
    seq_t result = 0;

    for (idx_t k = 1; k <= n; ++k) {
        idx_t max_t = n / k;
        seq_t powk = (seq_t)k;                /* k^t for current t */
        for (idx_t t = 1; t <= max_t; ++t) {
            /* C = n! / (n - k*t)! */
            seq_t C = fact_n / fact_cache[n - k * t];
            seq_t numer = C * powk;
            seq_t term = numer / (seq_t)t;
            result += term;
            powk *= (seq_t)k;                /* advance to next power k^(t+1) */
        }
    }

    return result;
}
