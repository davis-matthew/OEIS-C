/*
 * A371473  a(1) = 1; for n>1, if a(n-1) is squarefree, a(n) = a(n-1) + n,
 *          otherwise a(n) = squarefree kernel of a(n-1).
 * https://oeis.org/A371473
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 9714, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   ensure_capacity, is_squarefree, rad
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A371473
   a(1) = 1
   For n > 1,
      if a(n‑1) is squarefree   →   a(n) = a(n‑1) + n
      otherwise                 →   a(n) = rad(a(n‑1))
   (rad is the square‑free kernel, i.e., product of distinct prime
   factors.) */

#include <stddef.h>
#include "../utils/is_squarefree.h"
#include "../utils/rad.h"
#include "../utils/ensure_capacity.h"

static seq_t *seq_cache = NULL;   /* 1‑based cache of computed terms */
static size_t seq_cap   = 0;      /* current capacity of cache */
static idx_t  seq_len   = 0;      /* largest n such that seq_cache[n] is valid */

/* Return the nth term (n ≥ 1, offset 1).  Behaviour for n = 0 is undefined. */
seq_t a(idx_t n)
{
    if (n == 0)   /* offset is 1; behaviour below offset is irrelevant */
        return 0;

    if (n > seq_len) {
        /* Increase cache size if necessary */
        size_t new_cap = (size_t)n + 1; /* 1‑based indexing, keep room for n */
        ensure_capacity(new_cap, &seq_cap, (void **)&seq_cache,
                        sizeof(seq_t));

        /* Compute missing terms */
        for (idx_t i = seq_len + 1; i <= n; ++i) {
            seq_t prev = seq_cache[i - 1];
            if (is_squarefree(prev))
                seq_cache[i] = prev + (seq_t)i;
            else
                seq_cache[i] = rad((idx_t)prev);
        }
        seq_len = n;
    }

    return seq_cache[n];
}
