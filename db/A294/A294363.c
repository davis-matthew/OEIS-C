/*
 * A294363  E.g.f.: exp(Sum_{n>=1} d(n) * x^n), where d(n) is the number of
 *          divisors of n.
 * https://oeis.org/A294363
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)! * Sum_{k=1..n} k * d(k) * a(n-k)/(n-k)! Initial
 *            values: a(0) = 1. The function memoises results for efficiency
 *            and can be called with indices in any order
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

/* --------- static workspace --------------------------------- */
static seq_t *a_cache   = NULL;   /* memoised a(n) values                */
static uint32_t *divcnt = NULL;   /* divisor counts d(k)                 */
static idx_t cache_size = 0;      /* currently allocated size             */

/* Ensure workspace covers indices up to n (inclusive).  The divisor
   counts are recomputed completely each time the array is enlarged. */
static void ensure_workspace(idx_t n)
{
    if (n < cache_size)
        return;                                    /* already enough */

    idx_t new_size = (n + 1) * 2;                   /* grow by 2×       */

    seq_t *new_a = (seq_t *)realloc(a_cache, new_size * sizeof(seq_t));
    uint32_t *new_d = (uint32_t *)realloc(divcnt, new_size * sizeof(uint32_t));
    if (!new_a || !new_d)
        abort();                                   /* allocation failed */

    a_cache  = new_a;
    divcnt   = new_d;

    /* initialise all entries to zero */
    memset(new_d, 0, new_size * sizeof(uint32_t));
    for (idx_t i = cache_size; i < new_size; ++i)
        a_cache[i] = 0;

    /* compute divisor counts up to new_size (inclusive of 1). */
    for (idx_t i = 1; i < new_size; ++i)
        for (idx_t j = i; j < new_size; j += i)
            ++divcnt[j];

    a_cache[0] = (seq_t)1;              /* a(0) is always 1            */
    cache_size = new_size;
}

/* --------- public function --------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                   /* a(0) = 1 */

    ensure_workspace(n);

    for (idx_t m = 1; m <= n; ++m) {
        if (a_cache[m] != 0)               /* already computed */
            continue;

        seq_t sum = 0;
        seq_t r   = 1;                     /* r = (m-1)!/(m-k)! for the next k */

        for (idx_t k = 1; k <= m; ++k) {
            sum += (seq_t)k *
                   (seq_t)divcnt[k] *
                   a_cache[m - k] *
                   r;
            r *= (m - k);                  /* prepare r for k+1          */
        }
        a_cache[m] = sum;
    }

    return a_cache[n];
}
