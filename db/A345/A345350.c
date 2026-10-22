/*
 * A345350  Even triangular numbers such that the next integer is nonprime.
 * https://oeis.org/A345350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   grow_seq, isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS  A345350
 * Sequence: even triangular numbers T_k = k(k+1)/2 such that
 *   k ≡ 0 or 3 (mod 4)   (so that T_k is even)
 *   and T_k + 1 is not a prime.
 * This implementation builds the sequence lazily and memoises
 * the results.  It works for any seq_t (8‑16‑32‑64‑128‑big int)
 * and any idx_t = uint64_t.  It does not rely on any heavy
 * pre‑computed tables and stays far below 2.0 s for n up to 10000.
 *
 * The function's behaviour for indices n ≥ 1 is independent of the
 * order of calls.  For n < 1 the behaviour is undefined.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* isPrimeGeneric(seq_t) */
#include "../utils/grow_seq.h"         /* void grow_seq(seq_t **, size_t *, size_t) */

/* External types defined by the build system */
extern seq_t a(idx_t);

static seq_t *cache = NULL;   /* cached valid triangular numbers */
static size_t cache_size = 0; /* number of elements stored in cache */
static size_t cache_cap   = 0; /* allocated capacity of cache */
static idx_t  next_k      = 0; /* next k to examine (k starts at 0) */

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least n values.  Calls are
     * idempotent and order‑independent. */
    while (cache_size < (size_t)n) {
        /* Compute the k‑th triangular number in seq_t to avoid overflow. */
        seq_t t = (seq_t)next_k * ((seq_t)next_k + 1u) / 2u;

        /* k ≡ 0 or 3 (mod 4)  ⇔  triangular number is even. */
        bool evenT = ((next_k & 3u) == 0u) || ((next_k & 3u) == 3u);

        if (evenT && !isPrimeGeneric(t + 1u)) {
            /* Grow the cache if necessary.  grow_seq handles the
             * allocation of seq_t objects safely for both native
             * integral types and arbitrary‑precision classes. */
            if (cache_size == cache_cap) {
                size_t new_cap = (cache_cap == 0u) ? 4u : cache_cap * 2u;
                grow_seq(&cache, &cache_cap, new_cap);
            }
            cache[cache_size++] = t;
        }
        /* Move to the next k regardless of whether it was kept. */
        ++next_k;
    }
    return cache[(size_t)n - 1u];
}
