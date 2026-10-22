/*
 * A364776  Lexicographically least increasing sequence of triprimes whose
 *          first differences are semiprimes.
 * https://oeis.org/A364776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the lexicographically least increasing sequence of
 *            triprimes whose successive differences are semiprimes, defined
 *            as "a(1) = 8; for n ≥ 2, a(n) is the smallest triprime > a(n-1)
 *            such that a(n)-a(n-1) is a semiprime"
 * Range:     u8 n <= 21, u16 n <= 5435, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega, seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function a(n) uses a cache so that calls with any index order are
   efficient.  It relies on the helper bigomega() to test for a correct
   number of prime factors (exactly 3 for a triprime, exactly 2 for a
   semiprime).  The cache is stored in a dynamic array allocated with
   seq_alloc() and resized with seq_realloc().  Because seq_t may be an
   arbitrary‑precision integer class, the realloc logic uses seq_realloc(),
   which is defined to preserve the semantics of the class across resizes. */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL;   /* dynamic array of computed terms */
static size_t seq_len  = 0;        /* number of terms currently cached */
static size_t seq_cap  = 0;        /* allocated capacity of seq_cache */

/* Ensure the cache can store at least `min_cap` elements.  The array is
   grown multiplicatively to keep amortised O(1) insert operations. */
static void ensure_cache_capacity(size_t min_cap)
{
    if (seq_cap < min_cap) {
        size_t new_cap = seq_cap ? seq_cap * 2 : 256;
        if (new_cap < min_cap) new_cap = min_cap;
        seq_t *new_mem = seq_realloc(seq_cache, seq_cap, new_cap);
        seq_cache = new_mem;
        seq_cap   = new_cap;
    }
}

/* Return the n‑th term of OEIS sequence A364776.  The sequence is
   indexed starting at 1.  Calls may be in any order; the cache
   guarantees that each term is computed only once. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* offset is 1; callers will not request 0 */

    /* Initialise cache with first term a(1) = 8 */
    if (seq_len == 0) {
        ensure_cache_capacity(1);
        seq_cache[0] = 8;            /* a(1) */
        seq_len = 1;
    }

    /* Generate terms up to the requested index */
    while (seq_len < n) {
        seq_t prev = seq_cache[seq_len - 1];
        seq_t cand = prev + 1;

        for (;;) {
            /* Check whether cand is a triprime: exactly 3 prime factors */
            if (bigomega((idx_t)cand) == 3) {
                seq_t diff = cand - prev;
                /* Check whether diff is a semiprime: exactly 2 prime factors */
                if (bigomega((idx_t)diff) == 2) {
                    ensure_cache_capacity(seq_len + 1);
                    seq_cache[seq_len++] = cand;
                    break;   /* next term found */
                }
            }
            ++cand;
        }
    }

    return seq_cache[n - 1];
}
