/*
 * A358772  Numbers whose arithmetic derivative is of the form 4k+1, cf.
 *          A003415.
 * https://oeis.org/A358772
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   build_spf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = the n‑th integer whose arithmetic derivative ≡ 1 (mod 4)
   (OEIS A358772).  The function generates the numbers in ascending order
   on demand, caching them so that repeated calls are efficient.  The
   arithmetic derivative D(n) of n = ∏pᵉ is
      D(n) = ∑ (n / p) * e,
   which is an integer.  A number is accepted when D(n) % 4 == 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_spf.h"

/* Persistent cache of sequence terms */
static seq_t *cache      = NULL;
static size_t cache_len  = 0;
static size_t cache_cap  = 0;

/* Last integer that has been tested for the property */
static idx_t checked_up_to = 1;

/* Smallest prime factor table (0 for 0, 1 for 1) */
static idx_t *spf  = NULL;
static idx_t spf_limit = 0;

/* Ensure spf is computed up to at least 'limit' */
static void ensure_spf_up_to(idx_t limit)
{
    if (limit <= spf_limit) return;
    if (spf) free(spf);
    build_spf(limit, &spf);          /* allocates array of size limit+1 */
    spf_limit = limit;
}

/* Append a new term to the cache, growing the array as needed */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();           /* memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Return the n-th term of the sequence (1‑based).  Undefined for n==0. */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return 0;    /* safety; offset is 1 */

    /* Generate terms until we have at least n */
    while (cache_len < n) {
        /* Look for the next integer with the property */
        do {
            checked_up_to++;

            /* Make sure we can factorize this number */
            ensure_spf_up_to(checked_up_to);

            seq_t derivative = 0;
            idx_t nn = checked_up_to;
            seq_t n_seq = checked_up_to;

            while (nn > 1) {
                idx_t p = spf[nn];
                seq_t e = 0;
                while (nn % p == 0) {
                    nn /= p;
                    e++;
                }
                derivative += (n_seq / p) * e;
            }

            if ((derivative & 3) == 1) {
                cache_append(checked_up_to);
            }
        } while (cache_len < n);
    }

    return cache[n - 1];
}
