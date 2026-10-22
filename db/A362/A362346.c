/*
 * A362346  a(n) = n! * Sum_{k=0..floor(n/5)} (-n/120)^k /(k! * (n-5*k)!).
 * https://oeis.org/A362346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 10, i32 n <= 15, i64 n <= 24, i128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 *  Formula (OEIS A362346):
 *      a(n) = n! * sum_{k=0..floor(n/5)} (-n/120)^k / (k! * (n-5k)!).
 *
 *  The implementation evaluates the sum term‑by‑term, reducing the
 *  intermediate products by a gcd with the current denominator
 *  (120·k) to avoid overflow.  Results are cached so that any
 *  order of calls works efficiently.
 */

#include <stdlib.h>
#include <stdint.h>

/* Absolute value for seq_t */
static inline seq_t abs_seq(seq_t v)
{
    return v < 0 ? -v : v;
}

/* GCD for seq_t (works for signed integers) */
static inline seq_t mygcd(seq_t a, seq_t b)
{
    a = abs_seq(a);
    b = abs_seq(b);
    while (b != 0)
    {
        seq_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;   /* array of size cache_cap */
    static idx_t  cache_cap = 0;  /* number of elements stored */

    /* grow cache if necessary */
    if (n + 1 > cache_cap)
    {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return 0;                   /* allocation failure (unlikely) */
        cache = new_cache;

        /* compute new terms */
        for (idx_t i = cache_cap; i <= n; ++i)
        {
            idx_t kmax = i / 5;     /* floor(i/5) */
            seq_t sum  = 1;         /* term k = 0 */
            seq_t prev = 1;         /* term k-1, starting with 1 */

            for (idx_t k = 1; k <= kmax; ++k)
            {
                /* denominator = 120 * k */
                seq_t denom = 120;
                denom *= (seq_t)k;

                /* start from previous term */
                seq_t term = prev;

                /* factor -n */
                seq_t f = -(seq_t)n;
                term *= f;
                seq_t g = mygcd(term, denom);
                if (g > 1)
                {
                    term /= g;
                    denom /= g;
                }

                /* multiply the 5 terms (n-5k+5 .. n-5k+1) */
                idx_t start = i - 5 * k + 5;   /* first factor */
                for (idx_t j = 0; j < 5; ++j)
                {
                    f = (seq_t)(start - j);
                    term *= f;
                    g = mygcd(term, denom);
                    if (g > 1)
                    {
                        term /= g;
                        denom /= g;
                    }
                }
                /* after all factors denom should be 1 */
                sum  += term;
                prev = term;
            }
            cache[i] = sum;
        }
        cache_cap = new_cap;
    }
    return cache[n];
}
