/*
 * A369888  Sum of products of cubes of parts , counted without multiplicity,
 *          in all partitions of n.
 * https://oeis.org/A369888
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in \prod_{k\ge1} (1 + k^3 x^k/(1-x^k))
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 21, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t sz = (size_t)n + 1;
    seq_t *f = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *g = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!f || !g) {
        free(f);
        free(g);
        return (seq_t)0;   /* allocation failure sentinel */
    }

    /* f holds values using parts < current k; start with only part 0 */
    for (idx_t i = 0; i <= n; ++i)
        f[i] = (seq_t)0;
    f[0] = (seq_t)1;

    for (idx_t k = 1; k <= n; ++k) {
        seq_t k3 = (seq_t)k * (seq_t)k * (seq_t)k;

        for (idx_t r = 0; r < k; ++r) {
            seq_t cum = (seq_t)0;
            for (idx_t i = r; i <= n; i += k) {
                g[i] = f[i] + k3 * cum;
                cum += f[i];
            }
        }
        /* swap f and g for next iteration */
        seq_t *tmp = f;
        f = g;
        g = tmp;
    }

    seq_t result = f[n];

    free(g);  /* g now holds the old f */
    free(f);

    return result;
}
