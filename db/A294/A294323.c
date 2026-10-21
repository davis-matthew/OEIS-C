/*
 * A294323  a(n) = Product_{k=0..n} (5*k + 1)!.
 * https://oeis.org/A294323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=0..n} (5*k + 1)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 3,
 *            big any n
 * Helpers:   factorial, mul_small, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/seq_mul.h"
#include "../utils/mul_small.h"
#include "../utils/factorial.h"

static seq_t *cache = NULL;      /* array of computed a(k) */
static size_t cache_cap = 0;     /* allocated capacity of cache   */
static idx_t last_k = 0;         /* largest k already computed    */
static seq_t fact = 0;           /* (5*last_k + 1)!  */

/* Ensure that a(0..n) are computed and stored in cache. */
static void ensure_cache(idx_t n)
{
    /* first initialisation */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        fact   = factorial(1);          /* (5*0+1)! = 1! = 1   */
        cache[0] = 1;                   /* a(0) = 1            */
        last_k = 0;
        return;
    }

    /* compute any missing terms up to n */
    if (n > last_k) {
        for (idx_t k = last_k + 1; k <= n; ++k) {
            /* update fact from (5*(k-1)+1)! to (5*k+1)! */
            idx_t start = 5 * (k - 1) + 2;   /* first multiplier in the new block */
            for (idx_t i = 0; i < 5; ++i) {
                fact = mul_small(fact, (unsigned int)(start + i));
            }

            /* new a(k) = a(k-1) * fact */
            seq_t new_a = seq_mul(cache[k - 1], fact);

            /* grow cache if necessary */
            if ((size_t)k >= cache_cap) {
                size_t new_cap = cache_cap * 2;
                if (new_cap <= (size_t)k) new_cap = k + 1;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[k] = new_a;
        }
        last_k = n;
    }
}

/* public function required by the test harness */
seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
