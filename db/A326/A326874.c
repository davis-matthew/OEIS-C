/*
 * A326874  BII-numbers of abstract simplicial complexes.
 * https://oeis.org/A326874
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A326874: BII‑numbers of abstract simplicial complexes.
   The sequence enumerates the integers n such that the set of bit
   positions (starting from 1) of n is closed under taking submasks.
   The table is built by testing consecutive integers and caching
   the first 10 000 terms. */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/bitlen_seq_t.h"

/* Maximum number of terms to cache – sufficient for all
   calls with n ≤ 10 000. */
#define MAX_CACHE 10000

static idx_t cached_terms = 0;         /* number of cached terms */
static seq_t cache[MAX_CACHE];         /* cache[n‑1] = a(n) */
static seq_t next_candidate = 0;       /* next integer to test */

/* Check whether a candidate number satisfies the submask‑closure
   property needed for A326874. */
static bool is_valid(seq_t val)
{
    unsigned bits = bitlen_seq_t(val);   /* number of significant bits */
    for (idx_t l = 0; l < bits; ++l) {
        if ( ((val >> l) & 1) != 0 ) {      /* bit at position l is set */
            seq_t p   = (seq_t)(l + 1);     /* position in the set (1‑based) */
            seq_t sub = p;
            while (sub != 0) {
                idx_t bitPos = (idx_t)(sub - 1);   /* integer whose bit to test */
                if ( ((val >> bitPos) & 1) == 0 )
                    return false;                     /* required bit missing */
                sub = (sub - 1) & p;                 /* next submask of p */
            }
        }
    }
    return true;
}

/* Return the n-th term of A326874.  The sequence is 1‑based. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1; 0 is never requested */

    /* Generate terms until we have at least n cached. */
    while (cached_terms < n) {
        /* Skip candidates that are not valid. */
        while (!is_valid(next_candidate))
            ++next_candidate;

        cache[cached_terms] = next_candidate;
        ++cached_terms;
        ++next_candidate;
    }

    return cache[n - 1];
}
