/*
 * A385841  a(n) = 1 + Sum_{k=0..n-1} k^3 * a(k) * a(n-1-k).
 * https://oeis.org/A385841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + Sum_{k=0..n-1} k^3 * a(k) * a(n-1-k)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* initial values: a(0) = 1, a(1) = 1 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoized terms */
    static seq_t *seq = NULL;
    static idx_t size = 0;

    if (seq == NULL) {
        seq = (seq_t *)malloc(sizeof(seq_t));
        if (!seq) return (seq_t)0; /* allocation failed */
        seq[0] = (seq_t)1;
        size = 1;
    }

    if (n < size) return seq[n];

    /* extend cache to hold terms up to n */
    seq = (seq_t *)realloc(seq, sizeof(seq_t) * (n + 1));
    if (!seq) return (seq_t)0; /* allocation failed */

    for (idx_t m = size; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k < m; ++k) {
            seq_t k3 = (seq_t)k;
            k3 *= k3;          /* k^2 */
            k3 *= k;           /* k^3 */
            seq_t prod = k3 * seq[k] * seq[m - 1 - k];
            sum += prod;
        }
        seq[m] = sum + (seq_t)1;
    }

    size = n + 1;
    return seq[n];
}
