/* lucas: the Lucas number L(n), L(0) = 2.
 * Shared helper of the OEIS-C database (db/utils). */
#ifndef OEISC_UTILS_LUCAS_H
#define OEISC_UTILS_LUCAS_H
#include <stdbool.h>
#include <stdint.h>

/* Compute the m-th Lucas number (1‑based: L1 = 1, L2 = 3). */
static seq_t lucas(idx_t m)
{
    if (m == 0) return (seq_t)2; /* not used, but harmless */
    if (m == 1) return (seq_t)1;
    seq_t a = 1;   /* L1 */
    seq_t b = 3;   /* L2 */
    for (idx_t i = 3; i <= m; ++i) {
        seq_t tmp = a + b;
        a = b;
        b = tmp;
    }
    return b;
}

#endif
