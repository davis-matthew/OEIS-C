/*
 * A309044  Expansion of Product_{k>=0} (1 + x^(2^k) - x^(2^(k+1)))^2.
 * https://oeis.org/A309044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1; for n>0, a(n)=Σ_{k=0..4, (n-k) even} c_k * a((n-k)/2)
 *            with c=[1,2,-1,-2,1]. This follows from
 *            A(x)=(1+2x-x^2-2x^3+x^4)*A(x^2)
 * Range:     i8 n <= 69, i16 n <= 69, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;            /* cached a[0..cache_cap-1]     */
    static size_t cache_cap = 0;           /* allocated capacity           */
    static idx_t next_to_compute = 0;      /* first index not yet computed */

    /* return cached value if already known */
    if (n < next_to_compute) return cache[n];

    /* ensure the cache is large enough */
    size_t needed = (size_t)n + 1;
    size_t new_cap = cache_cap;
    if (new_cap == 0) new_cap = 1;
    while (new_cap < needed) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) return (seq_t)0;            /* allocation failure: return 0 */
    cache = tmp;
    cache_cap = new_cap;

    /* base value a(0)=1 */
    if (next_to_compute == 0) {
        cache[0] = (seq_t)1;
        next_to_compute = 1;
    }

    /* recurrence coefficients for (1+2x-x^2-2x^3+x^4) */
    static const seq_t coeff[5] = {
        (seq_t) 1,
        (seq_t) 2,
        (seq_t)-1,
        (seq_t)-2,
        (seq_t) 1
    };

    /* compute a[i] for i = next_to_compute .. n */
    for (idx_t i = next_to_compute; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (int k = 0; k < 5; ++k) {
            if ((idx_t)k > i) break;             /* k cannot exceed i          */
            if (((i - (idx_t)k) & (idx_t)1) == 0) { /* n-k even?                */
                idx_t j = (i - (idx_t)k) >> 1;   /* (i-k)/2                    */
                sum += coeff[k] * cache[j];
            }
        }
        cache[i] = sum;
    }
    next_to_compute = n + 1;
    return cache[n];
}
