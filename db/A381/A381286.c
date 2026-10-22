/*
 * A381286  Expansion of e.g.f. 1/(1 - sin(3*x) / 3).
 * https://oeis.org/A381286
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..floor((n-1)/2)} (-9)^k * binomial(n, 2*k+1) *
 *            a(n-2*k-1) Initial value: a(0) = 1
 * Range:     i8 n <= 4, i16 n <= 7, i32 n <= 11, i64 n <= 18, i128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/*  Cache for previously computed terms. */
static seq_t *cache = NULL;
static idx_t cache_cap = 0;   /* number of elements allocated in cache   */
static idx_t last_computed = 0;  /* highest n that has been computed so far */

/*  Compute a(n).  The function is memoised; any order of calls works. */
seq_t a(idx_t n)
{
    /* Allocate or grow the cache as needed. */
    if (cache == NULL) {
        cache_cap = n + 1;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        last_computed = 0;
    } else if (n + 1 > cache_cap) {
        cache_cap = n + 1;
        cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
    }

    /*  Compute any terms that are still missing. */
    if (last_computed < n) {
        for (idx_t m = last_computed + 1; m <= n; ++m) {
            if (m == 0) {
                cache[0] = (seq_t)1;   /* just in case */
                continue;
            }

            seq_t sum   = (seq_t)0;
            idx_t r     = 1;                         /* current odd index      */
            seq_t bc    = (seq_t)m;                  /* binomial(m,1)          */
            seq_t power = (seq_t)1;                  /* (-9)^0                 */

            while (1) {
                /*  term = power * bc * a(m-r)  */
                seq_t term = power * bc * cache[m - r];
                sum = sum + term;

                if (r + 2 > m) break;               /* finished all k's     */

                /*  Compute binomial(m, r+2) from binomial(m, r)  */
                seq_t num = (seq_t)(m - r) * (seq_t)(m - r - 1);
                seq_t den = (seq_t)(r + 1) * (seq_t)(r + 2);
                bc     = (bc * num) / den;

                /*  Next power:  (-9)^(k+1)  */
                power  = power * (seq_t)(-9);

                r += 2;
            }

            cache[m] = sum;
        }
        last_computed = n;
    }

    return cache[n];
}
