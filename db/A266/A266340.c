/*
 * A266340  G.f. = b(2)*b(4)*b(6)/(x^8+x^6-x^5+x^4-2*x^3-x+1), where b(k) =
 *          (1-x^k)/(1-x).
 * https://oeis.org/A266340
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in * ((1+x)(1+x+x^2+x^3)(1+x+...+x^5)
 *            / (x^8+x^6-x^5+x^4-2*x^3-x+1))
 * Range:     u8 n <= 8, u16 n <= 20, u32 n <= 44, u64 n <= 94, u128 n <= 192,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* this routine uses a linear recurrence derived from the rational generating function.
 * a_n = a_{n-1} + 2*a_{n-3} - a_{n-4} + a_{n-5} - a_{n-6} - a_{n-8} + b_n
 * with b_n = 1,3,5,7,8,8,7,5,3,1 for n = 0..9, and b_n = 0 for n ≥ 10.
 * The implementation memoizes computed values so that calls with arbitrary
 * indices arrive in any order.  All arithmetic is performed in seq_t,
 * which may be an arbitrary‑precision integer type or an unsigned integer.
 * Subtractions are performed after the sum of positive terms to avoid
 * unsigned underflow.  For the recurrence the final result is always non‑negative. */

#include <stdlib.h>

static const seq_t B[10] = { (seq_t)1, (seq_t)3, (seq_t)5, (seq_t)7,
                             (seq_t)8, (seq_t)8, (seq_t)7, (seq_t)5,
                             (seq_t)3, (seq_t)1 };

static seq_t *cache = NULL;   /* memoized sequence values */
static size_t cache_len = 0;  /* number of values stored (max index + 1) */
static size_t cache_cap = 0;  /* allocated capacity */

seq_t a(idx_t n)
{
    /* ensure cache for at least one element: a(0)=1 */
    if (cache == NULL) {
        cache_cap = 1;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;        /* allocation failed */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* if the requested index is already computed, just return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* enlarge cache if necessary */
    size_t need = (size_t)n + 1;
    if (need > cache_cap) {
        size_t newcap = cache_cap ? cache_cap : 1;
        while (newcap < need) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;              /* reallocation failed */
        cache = tmp;
        cache_cap = newcap;
    }

    /* compute terms from cache_len up to n */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        seq_t sum_plus  = 0;
        seq_t sum_minus = 0;

        if (i >= 1) sum_plus += cache[i - 1];
        if (i >= 3) {
            seq_t twice = cache[i - 3];
            twice += twice;        /* times 2 */
            sum_plus += twice;
        }
        if (i >= 5) sum_plus += cache[i - 5];
        if (i <= 9) sum_plus += B[i];   /* the inhomogeneous term */

        if (i >= 4) sum_minus += cache[i - 4];
        if (i >= 6) sum_minus += cache[i - 6];
        if (i >= 8) sum_minus += cache[i - 8];

        cache[i] = sum_plus - sum_minus;
    }

    cache_len = (size_t)n + 1;
    return cache[(size_t)n];
}
