/*
 * A374064  Expansion of Product_{k>=1} 1 / (1 + x^(3*k-1)).
 * https://oeis.org/A374064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of x^n in ∏_{k≥1} 1/(1 + x^(3*k-1)) (OEIS
 *            A374064: 1 + 0x - x^2 + 0x^3 + x^4 - x^5 - x^6 + …)
 * Range:     i8 n <= 75, i16 n <= 75, i32 n <= 75, i64 n <= 75, i128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#ifdef __cplusplus
#include <vector>
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 by definition */
    if (n == 0) return 1;

    size_t sz = (size_t)n + 1;            /* array length */

#ifdef __cplusplus
    std::vector<seq_t> buf(sz);           /* automatic construction in C++ */
#else
    seq_t *buf = calloc(sz, sizeof(seq_t)); /* zero‑init in C */
    if (!buf) return 0;                  /* allocation failure */
#endif

    buf[0] = 1;                           /* base case */

    /* iterate over part sizes s = 3*k - 1 that appear in the product */
    for (idx_t k = 1;; ++k) {
        idx_t s = 3 * k - 1;
        if (s > n) break;
        for (idx_t i = s; i <= n; ++i) {
            buf[i] = buf[i] - buf[i - s]; /* add new copies of size s, flipping sign */
        }
    }

    seq_t result = buf[n];

#ifndef __cplusplus
    free(buf);
#endif

    return result;
}
