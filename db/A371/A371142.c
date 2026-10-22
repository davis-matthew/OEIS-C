/*
 * A371142  E.g.f. satisfies A(x) = 1 + x*A(x)^2 * (exp(x) - 1).
 * https://oeis.org/A371142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   d(0) = 1
   d(n) = ∑_{k=0}^{n} binom(n,k) * a(k) * a(n-k)
   a(0) = 1, a(1) = 0
   a(n) = n * ∑_{s=0}^{n-2} binom(n-1,s) * d(s)   (n ≥ 2)
*/

#include <stdlib.h>
#include <stddef.h>
#include "../utils/seq_realloc.h"

static seq_t *seq_a = NULL;   /* a(n) values */
static seq_t *seq_d = NULL;   /* d(n) values */
static size_t seq_cap = 0;    /* current allocated capacity */
static idx_t  seq_used = 0;   /* number of values computed so far */

/* Ensure storage for indices up to n is available.  Use seq_realloc
   so that code works with arbitrary‑precision seq_t types as well. */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n + 1 <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 8;
    while ((size_t)n + 1 > new_cap)
        new_cap <<= 1;

    seq_a = seq_realloc(seq_a, seq_cap, new_cap);
    seq_d = seq_realloc(seq_d, seq_cap, new_cap);
    seq_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Allocate and initialise the cache on first use */
    ensure_capacity(n);
    if (seq_used == 0) {
        seq_a[0] = (seq_t)1;   /* a(0) */
        seq_d[0] = (seq_t)1;   /* d(0) = a0*a0 */
        seq_used = 1;
    }

    /* Compute all missing values up to n */
    while (seq_used <= n) {
        idx_t i = seq_used;   /* compute a(i) and d(i) */

        /* ---- compute a(i) ---- */
        seq_t sum = 0;
        if (i >= 2) {
            seq_t bin = 1; /* binom(i-1,0) */
            for (idx_t r = 0; r + 1 < i; ++r) {
                sum += seq_d[r] * bin;
                /* update bin to binom(i-1, r+1) */
                bin = bin * (seq_t)(i - 1 - r) / (seq_t)(r + 1);
            }
        }
        seq_a[i] = (seq_t)i * sum;

        /* ---- compute d(i) ---- */
        seq_t di = 0;
        seq_t bin = 1; /* binom(i,0) */
        for (idx_t u = 0; u <= i; ++u) {
            di += bin * seq_a[u] * seq_a[i - u];
            if (u < i)
                bin = bin * (seq_t)(i - u) / (seq_t)(u + 1);
        }
        seq_d[i] = di;

        ++seq_used;
    }

    return seq_a[n];
}
