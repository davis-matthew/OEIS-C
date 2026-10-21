/*
 * A245790  Number of preferential arrangements of n labeled elements when at
 *          least k=5 elements per rank are required.
 * https://oeis.org/A245790
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 a(n) = 0 for 1 ≤ n < 5 a(n) = Σ_{j=5}^{n} binom(n, j) ·
 *            a(n‑j) for n ≥ 5 where binom(n, j) = n!/(j!(n‑j)!)
 * Range:     u8 n <= 10, u16 n <= 14, u32 n <= 19, u64 n <= 29, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;   /* cache of computed terms */
    static idx_t max_n = 0;     /* highest index computed so far */

    /* Initialise cache for n = 0 */
    if (buf == NULL) {
        buf = (seq_t*)malloc(sizeof(seq_t));
        if (buf == NULL) return (seq_t)0;   /* allocation failed */
        buf[0] = (seq_t)1;
        max_n = 0;
    }

    /* Compute up to the requested n if necessary */
    if (n > max_n) {
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t*)realloc(buf, new_size * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failed */
        buf = tmp;

        for (idx_t i = max_n + 1; i <= n; ++i) {
            if (i < 5) {                       /* a(i) = 0 for 1 … 4 */
                buf[i] = (seq_t)0;
                continue;
            }

            /* i ≥ 5: use the recurrence */
            seq_t sum = (seq_t)0;

            /* binom(i,5) */
            seq_t binom = (seq_t)1;
            for (idx_t t = 1; t <= 5; ++t) {
                binom = binom * (seq_t)(i - t + 1) / (seq_t)t;
            }

            for (idx_t j = 5; j <= i; ++j) {
                sum += buf[i - j] * binom;
                if (j < i) {   /* update binom to binom(i, j+1) */
                    binom = binom * (seq_t)(i - j) / (seq_t)(j + 1);
                }
            }
            buf[i] = sum;
        }
        max_n = n;
    }

    return buf[n];
}
