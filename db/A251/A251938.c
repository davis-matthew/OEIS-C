/*
 * A251938  Number of length 4+2 0..n arrays with the sum of the maximum minus
 *          the median of adjacent triples multiplied by some arrangement of
 *          +-1 equal to zero.
 * https://oeis.org/A251938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) +3*a(n-3) -a(n-4) -2*a(n-5) -3*a(n-6) -3*a(n-7) *
 *            +5*a(n-8) +2*a(n-9) +5*a(n-10) -3*a(n-11) -3*a(n-12) *
 *            -2*a(n-13) -a(n-14) +3*a(n-15) +a(n-17) -a(n-18) * Initial
 *            values a(1)=34, a(2)=339, a(3)=1760, a(4)=6293, * a(5)=17598,
 *            a(6)=41677, a(7)=87328, a(8)=166677, a(9)=295898, * a(10)...
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 61, u64 n <= 94, u128 n <= 94,
 *            big any n
 * Helpers:   count_arrays, diff, has_partition, init_first_terms, max3, min3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/count_arrays.h"
#include "../utils/diff.h"
#include "../utils/has_partition.h"
#include "../utils/init_first_terms.h"
#include "../utils/max3.h"
#include "../utils/min3.h"

/* Main function computing the sequence. */
seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t allocated = 0;
    static idx_t current = 0;

    /* First call: allocate cache and compute first 18 terms. */
    if (!cache) {
        allocated = 64;
        cache = (seq_t *)malloc(allocated * sizeof(seq_t));
        if (!cache) return (seq_t)0;
        init_first_terms(cache);
        current = 18;
    }
    if (n == 0) return (seq_t)0;   /* undefined but guard */

    /* Grow cache if necessary. */
    if (n > current) {
        if ((size_t)(n + 1) > allocated) {
            size_t new_cap = allocated * 2;
            if (new_cap < (size_t)(n + 1)) new_cap = n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return (seq_t)0;
            cache = tmp;
            allocated = new_cap;
        }
        /* Compute missing terms using the recurrence. */
        for (idx_t k = current + 1; k <= n; ++k) {
            seq_t sum = cache[k-1];
            sum += cache[k-3] * (seq_t)3;
            sum -= cache[k-4];
            sum -= (seq_t)2 * cache[k-5];
            sum -= (seq_t)3 * cache[k-6];
            sum -= (seq_t)3 * cache[k-7];
            sum += (seq_t)5 * cache[k-8];
            sum += (seq_t)2 * cache[k-9];
            sum += (seq_t)5 * cache[k-10];
            sum -= (seq_t)3 * cache[k-11];
            sum -= (seq_t)3 * cache[k-12];
            sum -= (seq_t)2 * cache[k-13];
            sum -= cache[k-14];
            sum += (seq_t)3 * cache[k-15];
            sum += cache[k-17];
            sum -= cache[k-18];
            cache[k] = sum;
        }
        current = n;
    }
    return cache[n];
}
