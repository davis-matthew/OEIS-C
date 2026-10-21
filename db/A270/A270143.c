/*
 * A270143  a(n) = Sum_{k=0..n} (-1)^(k+1) * k * A000041(n-k).
 * https://oeis.org/A270143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(k+1) * k * A000041(n-k)
 * Range:     i8 n <= 18, i16 n <= 47, i32 n <= 134, i64 n <= 428,
 *            i128 n <= 1479, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cached partition numbers p(k) = A000041(k)  */
    static seq_t *partitions = NULL;
    static size_t cap = 0;          /* allocated size of partitions */
    static idx_t max_computed = 0;  /* largest index already computed */

    /* Adjust the cache size if necessary */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(partitions, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Out of memory – abort is safe for this helper function */
            abort();
        }
        partitions = tmp;
        cap = new_cap;
    }

    /* Ensure p(0) is defined */
    if (max_computed == 0 && partitions) {
        partitions[0] = (seq_t)1;
    }

    /* Compute partition numbers up to n using the pentagonal number theorem */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t total = 0;
        for (idx_t k = 1; ; ++k) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > i) break;
            seq_t sign = (k & 1) ? 1 : -1;          /* (-1)^(k+1) */
            total += sign * partitions[i - g1];

            idx_t g2 = k * (3 * k + 1) / 2;
            if (g2 <= i) {
                total += sign * partitions[i - g2];
            }
        }
        partitions[i] = total;
    }
    max_computed = n;

    /* Convolution sum */
    seq_t result = 0;
    for (idx_t k = 0; k <= n; ++k) {
        seq_t sign = (k & 1) ? 1 : -1;           /* (-1)^(k+1) */
        seq_t part = partitions[n - k];
        result += sign * ((seq_t)k * part);
    }

    return result;
}
