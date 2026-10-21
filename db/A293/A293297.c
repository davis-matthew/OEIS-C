/*
 * A293297  Row sums of A293472.
 * https://oeis.org/A293297
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=2, a(n)=sum_{k=1..n} C(n-1,k-1)*c_k*a(n-k) where
 *            c_1 = 2, c_k = (-1)^{k-2}*(k-2)! for k>=2. This recurrence
 *            follows from the definition a(n)=p(n,1) where p(n,t) is the n-th
 *            derivative of x^x (t = log x) evaluated at x=1
 * Range:     i8 n <= 5, i16 n <= 9, i32 n <= 14, i64 n <= 22, i128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache for a[0..] */
    static seq_t *cache = NULL;
    static size_t  cache_size = 0;   /* number of valid entries */
    static size_t  cache_cap  = 0;    /* capacity of cache */

    /* static cache for factorials (fact[i] = i!) */
    static seq_t *fact = NULL;
    static size_t  fact_cap  = 0;     /* capacity of fact array */
    static size_t  fact_size = 0;     /* number of valid factorials (last index +1) */

    /* Resize cache if necessary */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 128;
        while (new_cap < (size_t)n + 1)
            new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    /* Resize factorial array if necessary */
    if ((size_t)n + 2 > fact_cap) { /* need fact[0..n+1] */
        size_t new_cap = fact_cap ? fact_cap * 2 : 128;
        while (new_cap < (size_t)n + 2)
            new_cap <<= 1;
        fact = (seq_t *)realloc(fact, new_cap * sizeof(seq_t));
        fact_cap = new_cap;
    }

    /* Initialise cache and factorials once */
    if (cache_size == 0) {           /* cache is empty */
        cache[0] = (seq_t)1;         /* a(0)=1 */
        cache_size = 1;
    }
    if (fact_size == 0) {            /* factorial cache empty */
        fact[0] = (seq_t)1;          /* 0! = 1 */
        fact_size = 1;               /* next free index is 1 */
    }
    /* Compute missing factorials up to (n+1)! */
    if (fact_size <= (size_t)n + 1) {           /* need at least fact[n+1] */
        for (size_t i = fact_size; i <= (size_t)n + 1; ++i) {
            fact[i] = fact[i - 1] * (seq_t)i;
        }
        fact_size = (size_t)n + 2; /* next free index after n+1 */
    }

    /* Compute a(m) for all m from cache_size to n (inclusive) */
    for (size_t m = cache_size; m <= (size_t)n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;   /* C(m-1,0) */

        for (size_t k = 1; k <= m; ++k) {
            /* compute c_k */
            seq_t c;
            if (k == 1) {
                c = (seq_t)2;
            } else {
                seq_t val = fact[k - 2];          /* (k-2)! */
                if ((k - 2) & 1)                 /* (-1)^{k-2} */
                    val = -val;
                c = val;
            }

            sum += bin * c * cache[m - k];

            /* update bin for next k: bin = C(m-1,k) */
            if (k < m) {
                seq_t num = (seq_t)(m - k);   /* m-1-(k-1) = m-k */
                seq_t den = (seq_t)k;
                bin = (bin * num) / den;      /* exact division */
            }
        }
        cache[m] = sum;
    }
    cache_size = (size_t)n + 1;

    return cache[(size_t)n];
}
