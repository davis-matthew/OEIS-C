/*
 * A365913  Expansion of e.g.f. 1 / ( 1 - Sum_{k>=0} x^(3*k+4) / (3*k+4)! ).
 * https://oeis.org/A365913
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Σ_{k=0..⌊(n-4)/3⌋} C(n,3k+4)*a(n-3k-4), a(0)=1,
 *            a(1)=a(2)=a(3)=0. The function returns a(n) for any n≥0.
 *            Memoisation is used so that repeated calls are fast
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 18, u64 n <= 27, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"   // provides: seq_t binomial(idx_t n, idx_t k);

static seq_t *cache      = NULL;   // array holding computed values
static size_t cache_cap  = 0;      // number of valid entries in cache (size)

/* Ensure that cache contains values up to index n inclusively. */
static void ensure_up_to(idx_t n)
{
    /* cache has valid entries 0 .. cache_cap-1 */
    if (cache_cap > (size_t)n) return;

    /* Grow the cache to size n+1 (no exponential growth is required for the
       expected n up to 10 000). */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_ptr) abort();              // allocation failure

    cache = new_ptr;

    /* Compute values from the first missing index up to n. */
    for (idx_t i = (idx_t)cache_cap; i <= n; ++i) {
        if (i < 4) {                            // base cases
            cache[i] = (i == 0) ? (seq_t)1 : (seq_t)0;
        } else {
            seq_t sum = 0;
            idx_t maxk = (i - 4) / 3;
            for (idx_t k = 0; k <= maxk; ++k) {
                idx_t j = 3 * k + 4;           // j = 3k+4
                seq_t t = binomial(i, j);      // C(i,j)
                t *= cache[i - j];              // * a(i-j)
                sum += t;                       // accumulate
            }
            cache[i] = sum;
        }
    }

    cache_cap = new_cap;
}

/* External function required by the problem statement. */
seq_t a(idx_t n)
{
    ensure_up_to(n);
    return cache[n];
}
