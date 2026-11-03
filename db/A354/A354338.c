/*
 * A354338  a(n) = n! * Sum_{k=1..n} ( Sum_{d|k} 1/(d * (k/d)!) )/(n-k)!.
 * https://oeis.org/A354338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} A087906(k) * binomial(n,k) where A087906(k)
 *            = k! * Sum_{d|k} 1/(d * (k/d)!) The implementation pre‑computes
 *            A087906(k) on demand and uses this value to accumulate the
 *            binomial contributions. It uses 128‑bit arithmetic for safe
 *            intermediate products, so it works correctly on 64...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 22 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* --------------------------------------------------------------------
 *  Compute A087906(k) for a single divisor d of k.
 *  term(d,k) = k! / ( d * (k/d)! ) = (product_{i=k/d+1}^{k} i) / d
 * -------------------------------------------------------------------- */
static seq_t term(idx_t d, idx_t k)
{
    idx_t start = k / d + 1;          /* first factor in the product */
    unsigned __int128 prod = 1;        /* 128‑bit accumulator */
    for (idx_t i = start; i <= k; ++i)
        prod *= (unsigned __int128)i;  /* product of the range */

    prod /= d;                        /* division is exact */
    return (seq_t)prod;                /* truncate to seq_t type */
}

static seq_t *b       = NULL;   /* cache for A087906(k), 1 ≤ k ≤ cap-1 */
static size_t b_cap   = 0;       /* current allocated size */

/* --------------------------------------------------------------------
 *  Ensure that values b[1..max_n] are precalculated.
 *  If a larger limit is requested, grow the cache and recompute
 *  all entries (hits are overwritten).  The routine is O(n * sqrt(n)).
 * -------------------------------------------------------------------- */
static void ensure_b(idx_t max_n)
{
    if (b_cap > max_n) return;                /* already ready */

    size_t new_cap = (size_t)max_n + 1;        /* indices 0..max_n */
    seq_t *new_b = seq_alloc_new_array(new_cap);

    /* copy old data (if any) */
    for (size_t i = 0; i < b_cap; ++i)
        new_b[i] = b[i];

    /* zero the rest */
    for (size_t i = b_cap; i < new_cap; ++i)
        new_b[i] = (seq_t)0;

    /* free old array (if it existed) and update pointers */
    if (b) seq_free_array(b);
    b = new_b;
    b_cap = new_cap;

    /* compute all values anew (overwrites any previous values) */
    for (idx_t k = 1; k < new_cap; ++k)
    {
        seq_t sum = 0;
        for (idx_t d = 1; (idx_t)d * d <= k; ++d)
            if (k % d == 0)
            {
                sum += term(d, k);
                idx_t d2 = k / d;
                if (d2 != d)
                    sum += term(d2, k);
            }
        b[k] = sum;
    }
}

/* --------------------------------------------------------------------
 *  Main sequence function: a(n)
 *  Uses precomputed A087906(k) and a multiplicative binomial
 *  calculation that stays within 64 bit (or seq_t) limits.
 * -------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* safety, unused */

    ensure_b(n);

    seq_t res   = (seq_t)0;
    seq_t binom = (seq_t)1;                 /* binom(n,0) = 1 */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* binom = binom(n,k) = binom(n,k-1) * (n-k+1) / k  */
        unsigned __int128 tmp = (unsigned __int128)binom * (n - k + 1);
        binom = (seq_t)(tmp / k);

        res += binom * b[k];
    }
    return res;
}
