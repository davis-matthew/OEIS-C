/*
 * A337298  Sum of the coordinates of all relatively prime pairs of divisors
 *          of n, (d1,d2), such that d1 <= d2.
 * https://oeis.org/A337298
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i|n, k|n, i<=k, gcd(i,k)=1} (i+k)
 * Range:     u8 n <= 59, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/gcd.h"

static int cmp_idx(const void *p1, const void *p2) {
    idx_t a = *(const idx_t*)p1;
    idx_t b = *(const idx_t*)p2;
    return (a > b) - (a < b);
}

seq_t a(idx_t n) {
    if (n == 0) return 0;

    /* collect divisors of n */
    const size_t MAX_DIVS = 2048;
    idx_t divs[MAX_DIVS];
    size_t k = 0;

    for (idx_t d = 1; (seq_t)d * d <= n; ++d) {
        if (n % d == 0) {
            if (k < MAX_DIVS) divs[k++] = d;
            idx_t other = n / d;
            if (other != d && k < MAX_DIVS) divs[k++] = other;
        }
    }

    /* sort divisors in ascending order */
    qsort(divs, k, sizeof(idx_t), cmp_idx);

    /* sum over all valid pairs (i, j) with i <= j */
    seq_t sum = 0;
    for (size_t i = 0; i < k; ++i) {
        seq_t di = divs[i];
        for (size_t j = i; j < k; ++j) {
            seq_t dj = divs[j];
            if (gcd(di, dj) == 1) {
                sum += di + dj;
            }
        }
    }

    return sum;
}
