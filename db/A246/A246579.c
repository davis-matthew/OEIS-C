/*
 * A246579  G.f.: x^(k^2)/(mul(1-x^(2*i),i=1..k)*mul(1+x^(2*r-1),r=1..oo))
 *          with k=3.
 * https://oeis.org/A246579
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in x^9 / (Π_{i=1..3}(1 - x^{2i}) *
 *            Π_{r=1..∞}(1 + x^{2r-1}))
 * Range:     i8 n <= 23, i16 n <= 52, i32 n <= 52, i64 n <= 52, i128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Terms below 9 are zero because the numerator starts with x^9. */
    if (n < 9u)
        return 0;

    /* Work with indices up to needed = n - 9. */
    const idx_t needed = n - 9u;
    const idx_t new_cap = needed + 1u;   /* capacity needed for arrays */

    /* Static buffers that grow as needed. */
    static idx_t max_cap = 0;
    static seq_t *e = NULL;   /* partitions into parts {2,4,6}          */
    static seq_t *g = NULL;   /* partitions into distinct odd parts    */
    static seq_t *d = NULL;   /* inverse of g (series for 1/∏(1+x^odd))  */
    static seq_t *f = NULL;   /* convolution e*d (coefficients of G)    */

    /* Grow static arrays if necessary. */
    if (new_cap > max_cap) {
        max_cap = new_cap;
        e = (seq_t *)realloc(e, max_cap * sizeof(seq_t));
        g = (seq_t *)realloc(g, max_cap * sizeof(seq_t));
        d = (seq_t *)realloc(d, max_cap * sizeof(seq_t));
        f = (seq_t *)realloc(f, max_cap * sizeof(seq_t));
    }

    /* --- compute e: partitions into parts 2,4,6 (unrestricted) --- */
    e[0] = 1;
    for (idx_t i = 1; i < max_cap; ++i)
        e[i] = 0;
    const idx_t coins[3] = {2u, 4u, 6u};
    for (size_t ci = 0; ci < 3; ++ci) {
        idx_t coin = coins[ci];
        for (idx_t i = coin; i < max_cap; ++i)
            e[i] += e[i - coin];
    }

    /* --- compute g: partitions into distinct odd parts --- */
    g[0] = 1;
    for (idx_t i = 1; i < max_cap; ++i)
        g[i] = 0;
    for (idx_t part = 1; part < max_cap; part += 2) {
        for (idx_t i = max_cap - 1; i >= part; --i)
            g[i] += g[i - part];
    }

    /* --- compute d: inverse of g (series for 1/∏(1+x^odd)) --- */
    d[0] = 1;
    for (idx_t m = 1; m < max_cap; ++m) {
        seq_t sum = 0;
        for (idx_t k = 1; k <= m; ++k)
            sum += g[k] * d[m - k];
        d[m] = -sum;
    }

    /* --- compute f = e * d (convolution) --- */
    for (idx_t i = 0; i < max_cap; ++i)
        f[i] = 0;
    for (idx_t i = 0; i < max_cap; ++i) {
        if (e[i] == 0)
            continue;
        for (idx_t j = 0; j + i < max_cap; ++j)
            f[i + j] += e[i] * d[j];
    }

    /* Return the required coefficient a(n) = f[needed]. */
    return f[needed];
}
