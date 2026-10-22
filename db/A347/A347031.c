/*
 * A347031  a(n) = 1 - Sum_{k=2..n} (-1)^k * a(floor(n/k)).
 * https://oeis.org/A347031
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 324, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 1 - Σ_{k=2..n} (-1)^k * a(floor(n/k))
 * (OEIS A347031 – partial sums of A308077)
 */

#include <stdlib.h>
#include <stdbool.h>

/*  Static arrays for memoisation.
 *  cache[i] holds a(i) for i ≥ 1.  computed[i] is true iff cache[i] is valid.
 */
static seq_t  *cache      = NULL;
static bool   *computed   = NULL;
static size_t  cache_size = 0;          /* allocated size (≥ max_computed+1)   */
static size_t  max_computed = 0;        /* largest index that has been computed */

/*--- Utility ----------------------------------------------------------*/

static void
ensure_cache(idx_t n)
{
    size_t required = (size_t)n + 1;        /* indices 0 .. n   */
    if (required <= cache_size)
        return;

    /* grow to next power of two ≥ required */
    size_t new_size = cache_size ? cache_size << 1 : 1;
    while (new_size < required)
        new_size <<= 1;

    /* reallocate cache array */
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp)
        abort();
    cache = tmp;

    /* reallocate computed array */
    bool *tmpb = (bool *)realloc(computed, new_size * sizeof(bool));
    if (!tmpb)
        abort();
    /* initialise new entries to false */
    for (size_t i = cache_size; i < new_size; ++i)
        tmpb[i] = false;
    computed = tmpb;

    cache_size = new_size;
}

/*--- Compute terms up to 'up_to' if necessary -------------------------*/

static void
compute_missing(idx_t up_to)
{
    if (max_computed >= up_to)
        return;

    ensure_cache(up_to);

    /* initialise base case if needed */
    if (max_computed == 0) {
        cache[1]    = 1;          /* a(1) = 1 */
        computed[1] = true;
        max_computed = 1;
    }

    for (idx_t i = max_computed + 1; i <= up_to; ++i) {
        seq_t sum = 0;
        for (idx_t k = 2; k <= i; ++k) {
            seq_t v = cache[i / k];   /* a(floor(i/k)) is already known */
            if (k & 1)                /* odd k → (-1)^k = -1 */
                sum -= v;
            else
                sum += v;
        }
        cache[i]      = 1 - sum;
        computed[i]   = true;
        max_computed = i;
    }
}

/*--- Public accessor --------------------------------------------------*/

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for n = 0; return 0 to avoid UB */
    compute_missing(n);
    return cache[n];
}
