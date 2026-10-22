/*
 * A363267  Squares (A000290) alternating with centered squares (A001844).
 * https://oeis.org/A363267
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-2) - 3*a(n-4) + a(n-6) for n ≥ 7, with initial
 *            values: a(1) = 1, a(2) = 1, a(3) = 4, a(4) = 5, a(5) = 9, a(6) =
 *            13 (sequence A363267). The function memoises computed terms to
 *            allow arbitrary order of queries
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *seq_cache = NULL;
static idx_t seq_cache_size = 0;

/* Ensure cache contains values up to index n (1‑based). */
static void ensure_cache(idx_t n)
{
    if (n <= seq_cache_size) return;

    /* Resize cache.  New size is n elements (indices 1..n). */
    size_t new_size = (size_t)n;          /* assumes n <= SIZE_MAX */
    seq_t *new_cache = (seq_t *)realloc(seq_cache, new_size * sizeof(seq_t));
    if (!new_cache) abort();              /* out of memory: terminate */
    seq_cache = new_cache;

    /* Compute new entries. */
    for (idx_t i = seq_cache_size; i < n; ++i) {
        if (i < 6) {  /* base cases (i is 0‑based, a(i+1)) */
            switch ((int)i) {
            case 0: seq_cache[i] = (seq_t)1; break; /* a(1) */
            case 1: seq_cache[i] = (seq_t)1; break; /* a(2) */
            case 2: seq_cache[i] = (seq_t)4; break; /* a(3) */
            case 3: seq_cache[i] = (seq_t)5; break; /* a(4) */
            case 4: seq_cache[i] = (seq_t)9; break; /* a(5) */
            case 5: seq_cache[i] = (seq_t)13; break;/* a(6) */
            }
        } else {  /* recurrence a(i+1) = 3*a(i-1) - 3*a(i-3) + a(i-5) */
            seq_cache[i] =
                (seq_t)3 * seq_cache[i-2]
                - (seq_t)3 * seq_cache[i-4]
                + seq_cache[i-6];
        }
    }
    seq_cache_size = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* behaviour below offset does not matter */
    ensure_cache(n);
    return seq_cache[n-1];
}
