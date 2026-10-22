/*
 * A344133  a(n) = Sum_{i|n, j|n, k|n} i*j*k/gcd(i,j,k).
 * https://oeis.org/A344133
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Sum_{i|n, j|n, k|n} (i*j*k)/gcd(i,j,k)
 * Range:     u8 n <= 5, u16 n <= 23, u32 n <= 1259, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* cache values that have already been computed */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;

    if (n == 0)            /* the sequence starts at n = 1 */
        return 0;

    /* enlarge the cache if necessary */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap == 0) ? 1 : cache_cap;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {          /* allocation failure – ignore cache */
            cache_cap = 0;
            cache = NULL;
        } else {
            /* zero any newly added entries (use 0 as the “uncomputed” sentinel) */
            for (idx_t i = cache_cap; i < new_cap; ++i)
                tmp[i] = 0;
            cache = tmp;
            cache_cap = new_cap;
        }
    }

    /* return a cached value if it exists */
    if (cache && cache[n-1] != 0)
        return cache[n-1];

    /* build the list of divisors of n */
    size_t div_cnt = 0;
    size_t div_cap = 64;
    idx_t *divs = (idx_t *)malloc(div_cap * sizeof(idx_t));
    if (divs == NULL)
        return 0; /* allocation failed – fall back to 0 */

    for (idx_t d = 1; d <= n / d; ++d) {        /* use n/d to avoid overflow */
        if (n % d == 0) {
            if (div_cnt >= div_cap) {
                div_cap <<= 1;
                idx_t *tmp = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                if (tmp == NULL) { free(divs); return 0; }
                divs = tmp;
            }
            divs[div_cnt++] = d;

            idx_t e = n / d;
            if (e != d) {
                if (div_cnt >= div_cap) {
                    div_cap <<= 1;
                    idx_t *tmp = (idx_t *)realloc(divs, div_cap * sizeof(idx_t));
                    if (tmp == NULL) { free(divs); return 0; }
                    divs = tmp;
                }
                divs[div_cnt++] = e;
            }
        }
    }

    /* compute the triple sum */
    seq_t sum = 0;
    for (size_t ii = 0; ii < div_cnt; ++ii) {
        seq_t vi = divs[ii];
        for (size_t jj = 0; jj < div_cnt; ++jj) {
            seq_t vj = divs[jj];
            for (size_t kk = 0; kk < div_cnt; ++kk) {
                seq_t vk = divs[kk];
                seq_t prod = vi * vj * vk;
                seq_t g = gcd(vi, vj);
                g = gcd(g, vk);
                sum += prod / g;
            }
        }
    }

    free(divs);

    /* cache the result for future calls */
    if (cache)
        cache[n-1] = sum;

    return sum;
}
