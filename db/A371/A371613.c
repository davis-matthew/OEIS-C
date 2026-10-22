/*
 * A371613  G.f. satisfies A(x) = ( 1 + x / (1 - x*A(x)^3) )^2.
 * https://oeis.org/A371613
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=0..n} binomial(6*(n-k)+2, k) * binomial(n-1, n-k)
 *            / (3*(n-k)+1) (a(0)=1)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute A371613 by memoised summation and the provided binomial routine. */
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Special case for the offset term */
    if (n == 0)
        return (seq_t)1;

    static seq_t *cache = NULL;          /* cached values of a(i) */
    static idx_t cache_size = 0;         /* number of terms cached */

    /* Grow the cache to hold terms up to n */
    if (n + 1 > cache_size) {
        idx_t newsize = n + 1;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        for (idx_t i = cache_size; i < newsize; ++i) {
            seq_t sum = (seq_t)0;
            if (i == 0) {                /* already handled above, but kept for safety */
                cache[i] = (seq_t)1;
                continue;
            }
            for (idx_t k = 1; k <= i; ++k) {
                /* exp = i - k */
                idx_t exp = i - k;
                /* Skip if exp > i-1 (binomial would be zero) */
                if (exp > i - 1)
                    continue;

                idx_t a1 = 6 * exp + 2;   /* first upper binomial index */
                seq_t bin1 = binomial(a1, k);
                seq_t bin2 = binomial(i - 1, exp);
                seq_t denom = 3 * exp + 1;
                seq_t term = (bin1 * bin2) / denom;  /* division is exact */
                sum += term;
            }
            cache[i] = sum;
        }
        cache_size = newsize;
    }
    return cache[n];
}
