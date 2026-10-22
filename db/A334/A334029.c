/*
 * A334029  Length of the co-Lyndon factorization of the k-th composition in
 *          standard order.
 * https://oeis.org/A334029
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = length of the co‑Lyndon factorization of the n‑th
 *            composition in standard order (indexing starts at 0). The
 *            composition is obtained from the binary representation of n as
 *            described in the OEIS entry, and the factorization length is
 *            computed with a Duval‑like algorithm using reversed compar...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   co_lyndon_factor_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/co_lyndon_factor_len.h"

seq_t a(idx_t n)
{
    /* a(0) = 0 : empty composition */
    if (n == 0)
        return 0;

    /* Step 1: positions of set bits in n (LSB to MSB). */
    int pos[128];          /* more than enough for 64‑bit indices */
    idx_t nb = 0;
    idx_t tmp = n;
    idx_t bit_pos = 1;     /* 1‑based positions */
    while (tmp) {
        if (tmp & 1)
            pos[nb++] = (int)bit_pos;
        tmp >>= 1;
        ++bit_pos;
    }

    /* Step 2: build the composition from first differences and reverse. */
    int comp[128];
    idx_t m = nb;          /* number of parts */
    int prev = 0;
    for (idx_t i = 0; i < m; ++i) {
        int delta = pos[i] - prev;
        comp[m - 1 - i] = delta;   /* reverse order */
        prev = pos[i];
    }

    /* Step 3: compute the co‑Lyndon factorization length. */
    idx_t factors = co_lyndon_factor_len(comp, m);
    return (seq_t)factors;
}
