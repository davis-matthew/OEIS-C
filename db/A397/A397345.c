/*
 * A397345  O.g.f. satisfies A(x) = exp( x + Sum_{n>=2} (5*n^2 - 1) * a(n)*x^n
 *          / (5*n^2) ).
 * https://oeis.org/A397345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=a(1)=1 a(n)=5*n*a(n-1)+n*Σ_{k=1}^{n-2}
 *            ((5*(k+1)^2-1)/(k+1))*a(k+1)*a(n-k-1) for n>1 (recurrence (6)
 *            from OEIS A397345)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* external linkage */
seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;   /* array of terms */
    static idx_t cache_len = 0;   /* number of terms stored */

    /* enlarge cache if necessary */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        /* realloc may return void* in C++ */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_len * sizeof(seq_t));
        if (!tmp) {
            /* out of memory; terminate */
            exit(EXIT_FAILURE);
        }
        /* initialize new elements to 0 (uninitialized flag) */
        for (idx_t i = cache_len; i < new_len; ++i) {
            tmp[i] = 0;
        }
        cache = tmp;
        cache_len = new_len;
    }

    /* initialise base cases if not already set */
    if (cache_len >= 1 && cache[0] == 0) cache[0] = 1;
    if (cache_len >= 2 && cache[1] == 0) cache[1] = 1;

    /* compute terms up to n */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= i - 2; ++k) {
            idx_t m = k + 1;                     /* m = k+1  */
            seq_t mult = (seq_t)5 * (seq_t)m * (seq_t)m;   /* 5*m*m   */
            mult -= 1;                            /* 5*m*m-1 */
            seq_t prod = mult * cache[m] * cache[i - m];
            prod = prod / m;                      /* divide by (k+1) */
            sum += prod;
        }
        cache[i] = (seq_t)5 * (seq_t)i * cache[i - 1] + (seq_t)i * sum;
    }

    return cache[n];
}
