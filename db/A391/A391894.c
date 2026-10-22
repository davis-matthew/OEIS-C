/*
 * A391894  a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(2*k+1,2*n-2*k+1).
 * https://oeis.org/A391894
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 57,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A391894
    a(n) = Sum_{k=0..n} (k+1) * 2^k * binomial(2*k+1, 2*n-2*k+1)
    Recurrence for n ≥ 8:
        a(n) = 8*a(n-1) - 16*a(n-2) + 16*a(n-3) - 72*a(n-4)
               + 32*a(n-5) - 64*a(n-6) + 64*a(n-7) - 16*a(n-8)
    Initial terms (n = 0..7) are computed by the direct summation formula.
*/

#include <stdlib.h>
#include "../utils/binomial.h"   /* binomial(idx_t a, idx_t b) returns seq_t */

static seq_t *cache = NULL;     /* cache of already‑computed terms */
static idx_t cache_size = 0;    /* number of entries in cache */

/* direct summation formula, used for n < 8 */
static seq_t sum_formula(idx_t n)
{
    seq_t sum = 0;
    seq_t pow2 = 1; /* 2^0 */

    for (idx_t k = 0; k <= n; ++k) {
        idx_t top    = 2*k + 1;
        idx_t bottom = 2*n - 2*k + 1;
        seq_t bin    = binomial(top, bottom);
        seq_t term   = (seq_t)(k + 1) * pow2 * bin;
        sum += term;
        pow2 <<= 1; /* next power of two */
    }
    return sum;
}

/* recurrence computation for i ≥ 8, assumes cache[0..i-1] ready */
static seq_t recurrence(idx_t i)
{
    seq_t t = cache[i-1] * 8;
    t -= cache[i-2] * 16;
    t += cache[i-3] * 16;
    t -= cache[i-4] * 72;
    t += cache[i-5] * 32;
    t -= cache[i-6] * 64;
    t += cache[i-7] * 64;
    t -= cache[i-8] * 16;
    return t;
}

/* ensure cache contains terms up to index n */
static void ensure_cache(idx_t n)
{
    if (n < cache_size)
        return;                       /* already computed */

    size_t new_cap = (size_t)(n + 1);
    if (cache == NULL) {
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    } else {
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    }

    for (idx_t i = cache_size; i <= n; ++i) {
        if (i < 8) {
            cache[i] = sum_formula(i);
        } else {
            cache[i] = recurrence(i);
        }
    }
    cache_size = new_cap;
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
