/*
 * A369680  a(n) = Product_{k=0..n} (2^k + 3^(n-k)).
 * https://oeis.org/A369680
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{k=0..n} (2^k + 3^(n-k))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   grow_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation caches successive powers of 2 and 3 to avoid recomputing
   them.  For each call it recomputes the product from scratch.  The algorithm
   is O(n) per call in terms of loop iterations, but the cost of big integer
   multiplications dominates for large n.  It works for all n >= 0.  */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/grow_seq_array.h"     /* provided helper */

/* seq_t and idx_t are defined by the build.  */

/* Static caches for powers of 2 and 3, and for computed a(n).  */
static seq_t *pow2_cache = NULL;
static size_t pow2_cap = 0;

static seq_t *pow3_cache = NULL;
static size_t pow3_cap = 0;

static seq_t *a_cache = NULL;
static size_t a_cap = 0;

seq_t a(idx_t n)
{
    /* Ensure caches are large enough. */
    if (pow2_cap <= (size_t)n) {
        grow_seq_array(&pow2_cache, &pow2_cap, (size_t)n + 1);
        /* Fill new entries. */
        for (size_t i = pow2_cap > n ? 0 : pow2_cap; i <= (size_t)n; ++i) {
            if (i == 0) {
                pow2_cache[0] = (seq_t)1;          /* 2^0 */
            } else {
                pow2_cache[i] = pow2_cache[i - 1] * (seq_t)2;
            }
        }
        pow2_cap = (size_t)n + 1;
    }

    if (pow3_cap <= (size_t)n) {
        grow_seq_array(&pow3_cache, &pow3_cap, (size_t)n + 1);
        for (size_t i = pow3_cap > n ? 0 : pow3_cap; i <= (size_t)n; ++i) {
            if (i == 0) {
                pow3_cache[0] = (seq_t)1;          /* 3^0 */
            } else {
                pow3_cache[i] = pow3_cache[i - 1] * (seq_t)3;
            }
        }
        pow3_cap = (size_t)n + 1;
    }

    if (a_cap <= (size_t)n) {
        grow_seq_array(&a_cache, &a_cap, (size_t)n + 1);
    }

    /* Return cached value if available. */
    if (a_cache && n < a_cap && a_cache[n] != 0) {
        return a_cache[n];
    }

    /* Compute a(n) from scratch. */
    seq_t prod = (seq_t)1;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t term = pow2_cache[(size_t)k] + pow3_cache[(size_t)(n - k)];
        prod *= term;
    }

    /* Cache and return. */
    a_cache[(size_t)n] = prod;
    return prod;
}
