/*
 * A308669  a(1) = 1, a(2) = 3; for n > 1, a(n) = sum of the next two smallest
 *          integers > a(n-1) which are coprime to the sum s = a(1) + ... +
 *          a(n-1).
 * https://oeis.org/A308669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1, a(2)=3; for n>2, a(n) = the sum of the next two smallest
 *            * integers greater than a(n-1) that are coprime to the partial
 *            sum * s = a(1)+...+a(n-1). The function is memoised and works
 *            for any n>=1
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t *cache = NULL;      /* a[1..]  */
static seq_t *sum_cache = NULL;  /* prefix sums s[1..] = Σ_{i<=k} a(i) */
static idx_t  cache_cap = 0;     /* allocated capacity */
// keep count of highest computed index
static idx_t  last_computed = 0;

/* Ensure the allocated buffers are large enough for index 'needed' */
static void ensure_cap(idx_t needed)
{
    if (needed < cache_cap)
        return;
    idx_t new_cap = (cache_cap == 0) ? 4 : cache_cap;
    while (new_cap <= needed)
        new_cap *= 2;
    seq_t *new_cache = (seq_t*)malloc(new_cap * sizeof(seq_t));
    seq_t *new_sum   = (seq_t*)malloc(new_cap * sizeof(seq_t));
    if (cache)
        memcpy(new_cache, cache, cache_cap * sizeof(seq_t));
    if (sum_cache)
        memcpy(new_sum,   sum_cache,   cache_cap * sizeof(seq_t));
    free(cache);
    free(sum_cache);
    cache      = new_cache;
    sum_cache  = new_sum;
    cache_cap  = new_cap;
}

/* Compute terms up to and including index n (if not already computed) */
static void compute_up_to(idx_t n)
{
    if (n <= last_computed)
        return;
    ensure_cap(n);
    for (idx_t k = last_computed + 1; k <= n; ++k) {
        if (k == 1) {
            cache[k]     = (seq_t)1;
            sum_cache[k] = (seq_t)1;
        } else if (k == 2) {
            cache[k]     = (seq_t)3;
            sum_cache[k] = sum_cache[k-1] + cache[k];
        } else {
            seq_t prev = cache[k-1];
            seq_t s    = sum_cache[k-1];
            seq_t x1   = 0, x2   = 0;
            seq_t val  = prev + (seq_t)1;
            seq_t found = 0;
            while (found < 2) {
                if (gcd(val, s) == (seq_t)1) {
                    if (found == 0) x1 = val;
                    else            x2 = val;
                    ++found;
                }
                val += (seq_t)1;
            }
            cache[k]     = x1 + x2;
            sum_cache[k] = sum_cache[k-1] + cache[k];
        }
    }
    last_computed = n;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)
        return (seq_t)0;
    compute_up_to(n);
    return cache[n];
}
