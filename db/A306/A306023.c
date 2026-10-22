/*
 * A306023  Stirling transform of partitions into distinct parts (A000009).
 * https://oeis.org/A306023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} Stirling2(n,k) * A000009(k)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 41,
 *            big any n
 * Helpers:   seq_alloc_new_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <string.h>
#include "../utils/seq_alloc_new_array.h"
#include "../utils/seq_free_array.h"

/* Static storage for memoisation */
static seq_t *part   = NULL;  /* A000009(k) for 0 ≤ k ≤ current capacity-1 */
static seq_t *aseq   = NULL;  /* desired sequence values a(k)          */
static seq_t *Sprev  = NULL;  /* previous Stirling‑2 row (for n-1)    */
static seq_t *Snext  = NULL;  /* temporary Stirling‑2 row (for n)     */
static idx_t cap     = 0;     /* capacity of the arrays               */
static idx_t max_n   = (idx_t)-1; /* largest n computed so far          */

/* Grow all tables to satisfy a new minimum capacity */
static void grow_tables(idx_t new_cap)
{
    /* Allocate new storage */
    seq_t *new_part  = seq_alloc_new_array(new_cap);
    seq_t *new_aseq  = seq_alloc_new_array(new_cap);
    seq_t *new_Sprev = seq_alloc_new_array(new_cap);
    seq_t *new_Snext = seq_alloc_new_array(new_cap);

    /* Copy existing data if present */
    if (part) {
        size_t copy_len = (cap < new_cap) ? cap : new_cap;
        memcpy(new_part,  part,  copy_len * sizeof(seq_t));
        memcpy(new_aseq,  aseq,  copy_len * sizeof(seq_t));
        memcpy(new_Sprev, Sprev, copy_len * sizeof(seq_t));
        /* Snext will be overwritten, no need to copy */
    }

    /* Initialise new entries to zero */
    for (idx_t i = cap; i < new_cap; ++i) {
        new_part[i]  = (seq_t)0;
        new_aseq[i]  = (seq_t)0;
        new_Sprev[i] = (seq_t)0;
        new_Snext[i] = (seq_t)0;
    }

    /* Free old storage */
    if (part)   seq_free_array(part);
    if (aseq)   seq_free_array(aseq);
    if (Sprev)  seq_free_array(Sprev);
    if (Snext)  seq_free_array(Snext);

    /* Commit new pointers and capacity */
    part   = new_part;
    aseq   = new_aseq;
    Sprev  = new_Sprev;
    Snext  = new_Snext;
    cap    = new_cap;
}

/* Public function ----------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Initialise internal tables on first call */
    if (max_n == (idx_t)-1) {
        grow_tables(1);                 /* capacity at least 1 */
        part[0]   = (seq_t)1;           /* A000009(0) = 1      */
        aseq[0]   = (seq_t)1;           /* a(0) = 1            */
        Sprev[0]  = (seq_t)1;           /* Stirling row for 0   */
        max_n     = 0;
    }

    /* Expand if needed for the requested index */
    if (n > max_n) {
        /* Ensure capacity */
        if (n + 1 > cap) {
            grow_tables(n + 1);
        }

        /* ---- 1. Compute partitions into distinct parts up to n ---- */
        part[0] = (seq_t)1;            /* reset base case   */
        for (idx_t i = 1; i <= n; ++i) {
            part[i] = (seq_t)0;
        }
        for (idx_t p = 1; p <= n; ++p) {
            for (idx_t s = n; s >= p; --s) {
                part[s] = part[s] + part[s - p];
                if (s == p) break;    /* avoid underflow on unsigned */
            }
        }

        /* ---- 2. Extend Stirling‑2 rows and compute new a(m) values -- */
        for (idx_t m = max_n + 1; m <= n; ++m) {
            /* Compute row m into Snext */
            Snext[0] = (seq_t)0;
            for (idx_t k = 1; k <= m; ++k) {
                seq_t term1 = Sprev[k - 1];
                seq_t term2 = (k <= m - 1) ? (Sprev[k] * (seq_t)k) : (seq_t)0;
                Snext[k] = term1 + term2;
            }

            /* a(m) = Σ_k Snext[k] * part[k] */
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k <= m; ++k) {
                sum = sum + (Snext[k] * part[k]);
            }
            aseq[m] = sum;

            /* Prepare for next iteration */
            seq_t *tmp = Sprev;
            Sprev = Snext;
            Snext = tmp;
        }

        max_n = n;    /* record highest n computed */
    }

    return aseq[n];
}
