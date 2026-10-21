/*
 * A259896  Expansion of psi(x) * psi(x^6) in powers of x where psi() is a
 *          Ramanujan theta function.
 * https://oeis.org/A259896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (i,j) with i≥0, j≥0 such that * i(i+1)/2
 *            + 3·j(j+1) = n * Equivalently, the number of positive odd
 *            solutions (x,y) to * x² + 6·y² = 8 n + 7
 * Range:     u8 n <= 373, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bitlen_seq_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/bitlen_seq_t.h"

/* Integer square root for seq_t using a binary‑search on bits. */
static seq_t int_sqrt(seq_t n)
{
    if (n < 2)
        return n;

    seq_t r = 0;
    seq_t bit = ((seq_t)1) << ((bitlen_seq_t(n) - 1) / 2);

    while (bit) {
        seq_t res = r + bit;
        if (res <= n / res)
            r = res;
        bit >>= 1;
    }
    return r;
}

/* Main sequence evaluation. */
seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t T;      /* triangular number i(i+1)/2 */

    for (idx_t i = 0; ; ++i) {
        T = (seq_t)i * (seq_t)(i + 1) / 2;
        if (T > (seq_t)n)
            break;

        seq_t rem = (seq_t)n - T;
        if (rem % 3 != 0)
            continue;

        seq_t r = rem / 3;                      /* want j(j+1) = r */
        seq_t D = 1 + 4 * r;                    /* discriminant */
        seq_t sqrtD = int_sqrt(D);
        if (sqrtD * sqrtD != D)
            continue;

        seq_t j = (sqrtD - 1) / 2;              /* j = (-1 + sqrt(D))/2 */
        if (j * (j + 1) != r)                   /* safety check */
            continue;

        ++count;
    }

    return count;
}
