/*
 * A359527  Nonnegative numbers k such that if 2^i and 2^j appear in the
 *          binary expansion of k, then 2^(i OR j) also appears in the binary
 *          expansion of k (where OR denotes the bitwise OR operator).
 * https://oeis.org/A359527
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth nonnegative integer whose set of binary
 *            exponents is closed under bitwise OR (OEIS A359527). The
 *            implementation tests each integer for this closure property
 * Range:     u8 n <= 60, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   grow_seq_array, union_closed_ok
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq_array.h"
#include "../utils/union_closed_ok.h"

/* Cached sequence and state. */
static seq_t *cache   = NULL;   /* array of computed terms */
static idx_t  cache_len = 0;    /* number of terms stored */
static idx_t  cache_cap = 0;    /* capacity of the array */
static seq_t  last_candidate = 0;  /* next integer to test */

/* Ensure that the cache contains at least n terms. */
static void ensure_cache(idx_t n)
{
    while (cache_len < n) {
        /* Grow the cache if needed. */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
            grow_seq_array(&cache, &cache_cap, new_cap);
        }

        /* Test the current candidate. */
        if (union_closed_ok(last_candidate))
            cache[cache_len++] = last_candidate;

        /* Move to the next integer. */
        last_candidate++;
    }
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n == 0)                /* offset is 1: a(1)=0 */
        return 0;
    ensure_cache(n);
    return cache[n - 1];
}
