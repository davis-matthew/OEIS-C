/*
 * A357347  E.g.f. satisfies A(x) = (exp(x * exp(A(x))) - 1) * exp(2 * A(x)).
 * https://oeis.org/A357347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k=1..n} (n+2*k)^(k-1) * Stirling2(n,k) (OEIS A357347)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 17,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/big_pow.h"

/* Persistent storage for the sequence and for Stirling‑2 numbers */
static seq_t *seq_cache   = NULL;   /* a(n) cache */
static seq_t *Stirling    = NULL;   /* current row of Stirling‑2 numbers */
static idx_t  max_n       = 0;      /* greatest n computed so far */
static size_t cap         = 0;      /* allocated capacity */

static void ensure_capacity(size_t needed)
{
    if (cap >= needed)
        return;

    size_t new_cap = cap ? (cap * 2) : needed;
    if (new_cap < needed)
        new_cap = needed;

    /* Allocate new arrays; use C‑style cast to avoid C++ warnings */
    seq_t *new_stirling = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_cache    = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (!new_stirling || !new_cache) {
        /* In an OJ environment, an allocation failure is fatal */
        exit(EXIT_FAILURE);
    }

    /* Copy existing elements */
    for (size_t i = 0; i < cap; ++i) {
        new_stirling[i] = Stirling[i];
        new_cache[i]    = seq_cache[i];
    }

    /* Initialise any new elements to zero */
    for (size_t i = cap; i < new_cap; ++i) {
        new_stirling[i] = (seq_t)0;
        new_cache[i]    = (seq_t)0;
    }

    /* Initialise Stirling‑2(0,0)=1 only if this is the first allocation */
    if (cap == 0 && new_cap > 0)
        new_stirling[0] = (seq_t)1;

    /* Free old arrays */
    free(Stirling);
    free(seq_cache);

    Stirling   = new_stirling;
    seq_cache  = new_cache;
    cap        = new_cap;
}

/* Compute the n‑th term of A357347.  The function is memoised and works
   for calls in arbitrary order. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;     /* a(0) = 0 */

    /* Grow the cache if necessary */
    if (max_n < n) {
        ensure_capacity((size_t)n + 1);   /* indices 0..n inclusive */

        for (idx_t cur = max_n + 1; cur <= n; ++cur) {
            /* Compute Stirling‑2(cur, k) for k=cur..1 using the recurrence
               S(cur,k) = k * S(cur-1,k) + S(cur-1,k-1). */
            for (idx_t k = cur; k >= 1; --k) {
                Stirling[k] = Stirling[k] * (seq_t)k + Stirling[k - 1];
            }
            /* For n>0, S(cur,0)=0 */
            Stirling[0] = (seq_t)0;

            /* Evaluate the sum a(cur) = Σ_{k=1..cur} (cur+2k)^(k-1) * S(cur,k) */
            seq_t sum = (seq_t)0;
            for (idx_t k = 1; k <= cur; ++k) {
                seq_t base   = (seq_t)cur + (seq_t)(2 * k);
                seq_t power  = big_pow(base, (seq_t)(k - 1));
                sum += power * Stirling[k];
            }
            seq_cache[cur] = sum;
        }
        max_n = n;
    }

    return seq_cache[n];
}
