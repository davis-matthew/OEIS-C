/*
 * A358774  Numbers whose arithmetic derivative is of the form 4k+3, cf.
 *          A003415.
 * https://oeis.org/A358774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 48, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   derivative_mod4_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A358774: numbers whose arithmetic derivative is congruent to 3 (mod 4) */
/* The arithmetic derivative d(n) satisfies d(p^e) = e·p^{e-1} for a prime p,
   and d(ab) = a·d(b) + b·d(a).  Hence               d(n) = n·∑_{p|n} e_p/p     */
/* This implementation computes terms on demand, caching all values found
   so far. The arithmetic derivative is computed modulo 4 by factoring
   candidate integers with a simple trial division routine.  The code
   assumes that the values of this sequence for n ≤ 10000 are less than
   2^64, which is true for the current OEIS data set.  The algorithm
   runs in amortised O(1) time per call with almost linear scan
   over the integers. */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/derivative_mod4_uint64.h"

/* types provided by the build system */
extern seq_t a(idx_t n);

/* ------------------------------------------------------------------ */

/* ------------------------------------------------------------------ */
/* Cache state for the sequence.  All values are stored in the array
   seq_cache, which grows as needed.  The variable last_checked keeps
   track of the largest integer examined so far.  */
static seq_t *seq_cache = NULL;   /* array of sequence values            */
static size_t   seq_len   = 0;     /* number of values currently cached   */
static size_t   seq_cap   = 0;     /* current allocated capacity          */
static uint64_t last_checked = 1;  /* last integer examined (≥ 1)         */

/* ------------------------------------------------------------------ */
/* Main entry point for the OEIS driver.  The function is called
   arbitrarily many times with any order of n ≥ 1.  It guarantees to
   return the correct a(n). */
seq_t a(idx_t n)
{
    /* Ensure cache is initialized */
    if (seq_cache == NULL) {
        /* a reasonable initial capacity; will grow as needed */
        seq_cap = (size_t)(n > 1024 ? n : 1024);
        seq_cache = (seq_t *)malloc(seq_cap * sizeof(seq_t));
        seq_len = 0;
        last_checked = 1;
    }

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {
        last_checked++;                         /* next candidate */
        if (derivative_mod4_uint64(last_checked) == 3) {
            /* store it */
            if (seq_len == seq_cap) {
                /* double capacity */
                seq_cap *= 2;
                seq_cache = (seq_t *)realloc(seq_cache, seq_cap * sizeof(seq_t));
            }
            seq_cache[seq_len++] = (seq_t)last_checked;
        }
    }

    return seq_cache[(size_t)n - 1];
}
