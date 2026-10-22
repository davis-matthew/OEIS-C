/*
 * A318600  a(1) = 1; a(n+1) = Sum_{d|n} (-1)^(n/d+1)*d*a(d).
 * https://oeis.org/A318600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 21, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; for n≥1,
   a(n+1) = Σ_{d|n} (-1)^(n/d+1) * d * a(d) */
#include <stdint.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* dynamically allocated cache of a(n) */
static idx_t  cache_alloc = 0;       /* size of the allocated array (inclusive of 0) */
static idx_t  cache_computed = 0;    /* largest index for which a(i) is already stored */

/* Ensure that the cache can hold values up to ``max`` (inclusive).  New
 * entries are zero‑initialised. */
static void
ensure_cache(idx_t max)
{
    if (max < cache_alloc)
        return;
    size_t new_alloc = (size_t)(max + 1);
    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!tmp)
        abort();  /* out of memory */
    /* zero the newly allocated part */
    for (size_t i = cache_alloc; i < new_alloc; ++i)
        tmp[i] = (seq_t)0;
    cache = tmp;
    cache_alloc = new_alloc;
}

/* Return the n‑th term of A318600 for any n ≥ 1.  The function is
 * memoised and works for any calling order. */
seq_t
a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined, but safe */

    ensure_cache(n);

    /* Initialise the first term once */
    if (cache_computed == 0) {
        cache[1] = (seq_t)1;
        cache_computed = 1;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        idx_t m = i - 1;                /* the argument of the recurrence */
        seq_t pos_sum = (seq_t)0;
        seq_t neg_sum = (seq_t)0;

        /* Enumerate all divisors of m */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                /* divisor d */
                seq_t term = (seq_t)d * cache[d];
                idx_t exp = (m / d) + 1;
                if (exp & 1ULL)
                    neg_sum += term;
                else
                    pos_sum += term;

                /* complementary divisor m/d, if distinct */
                idx_t d2 = m / d;
                if (d2 != d) {
                    term = (seq_t)d2 * cache[d2];
                    exp = (m / d2) + 1;
                    if (exp & 1ULL)
                        neg_sum += term;
                    else
                        pos_sum += term;
                }
            }
        }

        /* The definition guarantees a(n) > 0, so pos_sum >= neg_sum. */
        cache[i] = pos_sum - neg_sum;
    }

    cache_computed = n;
    return cache[n];
}
