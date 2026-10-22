/*
 * A353384  Irregular triangle T(n,k) with row n listing A003592(j) not
 *          divisible by 20 such that A352218(A003592(j)) = n.
 * https://oeis.org/A353384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base, tabf
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 25, u32 n <= 54, u64 n <= 110,
 *            u128 n <= 218, big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A353384: Irregular triangle T(n,k)
 * For n = 0: a(0) = 1
 * For n ≥ 1:
 *   let r = (n + 3) / 4           // row number (≥1)
 *   values = { 2^(2r−1), 5^r, 2^(2r), 2·5^r }
 *   sort these four values
 *   a(n) is the ((n−1) mod 4)-th element of the sorted list
 *
 * The computation uses seq_pow from "../utils/seq_pow.h".
 */
#include "../utils/seq_pow.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* row number for n ≥ 1 */
    idx_t r = (n + 3) / 4;

    /* compute the four base values */
    seq_t v1 = seq_pow((seq_t)2, (idx_t)(2 * r - 1)); /* 2^(2r-1) */
    seq_t v2 = seq_pow((seq_t)5, r);                  /* 5^r      */
    seq_t v3 = seq_pow((seq_t)2, (idx_t)(2 * r));     /* 2^(2r)   */
    seq_t v4 = v2 * (seq_t)2;                         /* 2·5^r    */

    /* store in small array and sort ascending */
    seq_t vals[4] = {v1, v2, v3, v4};
    for (idx_t i = 0; i < 3; ++i)
        for (idx_t j = i + 1; j < 4; ++j)
            if (vals[j] < vals[i]) {
                seq_t tmp = vals[i];
                vals[i] = vals[j];
                vals[j] = tmp;
            }

    /* select the term in the row */
    uint8_t k = (uint8_t)((n - 1) % 4);
    return vals[k];
}
