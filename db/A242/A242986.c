/*
 * A242986  a(n) = 6*(n+1)!/((3+floor(n/2))*(floor(n/2)!)^2).
 * https://oeis.org/A242986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*(n+1)!/((3+floor(n/2))*(floor(n/2)!)^2) Recurrence
 *            (n>0): if n is odd : a(n) = a(n-1)*(n+1) if n is even : a(n) =
 *            a(n-1)*(n+1)*(4*(n+4))/(n^2*(n+6)) a(0) = 2
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 19, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[0..last] holds a(k) */
static idx_t  last  = 0;

seq_t a(idx_t n)
{
    /* Grow cache if needed */
    if (cache == NULL) {
        cache = (seq_t*)malloc((n + 1) * sizeof(seq_t));
        if (cache == NULL) abort();
        cache[0] = (seq_t)2;    /* a(0) */
        last = 0;
    }
    if (last < n) {
        size_t newsize = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
        for (idx_t k = last + 1; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)2;
                continue;
            }
            if ((k & 1) == 0) {   /* even */
                seq_t num = cache[k - 1];
                num *= (seq_t)(k + 1);
                num *= (seq_t)(4 * (k + 4));
                seq_t den = (seq_t)(k * k * (k + 6));
                cache[k] = num / den;
            } else {              /* odd */
                cache[k] = cache[k - 1] * (seq_t)(k + 1);
            }
        }
        last = n;
    }
    return cache[n];
}
