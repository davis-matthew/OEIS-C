/*
 * A348368  Numbers k such that w(k + w(k)) < w(k), where w(k) is the binary
 *          weight of k, A000120(k).
 * https://oeis.org/A348368
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   Terms k such that w(k + w(k)) < w(k), where w(k) is the binary
 *            weight (number of 1‑bits) of k. The function a(n) returns the
 *            nth term (a(1) = 6). The implementation searches for terms in
 *            increasing order and memoises them in a static array so that
 *            repeated calls with arbitrarily ordered indices a...
 * Range:     u8 n <= 82, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   popcount_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/popcount_seq_t.h"

/* popcount for any unsigned seq_t (works for native and arbitrary‑precision
   types that support &, |, >>, +, -, <, >, == and 0). */

/* Static cache of found terms                                 */
static seq_t *seq_cache = NULL;   /* array of terms */
static size_t seq_len  = 0;       /* number of stored terms */
static size_t seq_cap  = 0;       /* allocated capacity     */

/* Main sequence function                                       */
seq_t a(idx_t n)
{
    if (n == 0)              /* undefined for 0, return 0 safely */
        return (seq_t)0;

    /* Generate terms until we have at least n of them */
    while (seq_len < (size_t)n) {

        /* Grow the cache if necessary */
        if (seq_len == seq_cap) {
            size_t new_cap = (seq_cap ? seq_cap * 2 : 16);
            seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
            if (!tmp)
                abort();          /* allocation failed */
            seq_cache = tmp;
            seq_cap   = new_cap;
        }

        /* Search for the next valid k, starting after the latest
           found value (stored in next_k). */
        static seq_t next_k = 1;   /* candidate start after last found */
        for (seq_t k = next_k; ; ++k) {
            unsigned int w  = popcount_seq_t(k);
            seq_t sum       = k + (seq_t)w;
            unsigned int ws = popcount_seq_t(sum);
            if (ws < w) {           /* k satisfies the condition   */
                seq_cache[seq_len++] = k;
                next_k = k + 1;      /* next search starts after k  */
                break;               /* found next term              */
            }
        }
    }

    return seq_cache[(size_t)n - 1];
}
