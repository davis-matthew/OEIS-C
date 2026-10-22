/*
 * A371276  Nonnegative numbers whose balanced ternary expansions have no
 *          consecutive equal digits (with offset 0).
 * https://oeis.org/A371276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   For n = 0, a(n) = 0. For n > 0, let the binary expansion of n be
 *            (b_1,…,b_w) with b_1 = 1. Construct a balanced‑ternary expansion
 *            (t_1,…,t_w) as follows: t_1 = 1. For i = 2 … w, let S_i = { –1,
 *            0, 1 } \ { t_{i‑1} } sorted increasingly. Then t_i = S_i[ b_i ]
 *            (0–based indexing: 0 → smallest element...
 * Range:     u8 n <= 55, u16 n <= 1983, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* a(0) = 0 */

    /* Determine the number of bits in n. */
    idx_t tmp = n;
    idx_t w = 0;
    while (tmp) { ++w; tmp >>= 1; }

    /* First ternary digit is always 1. */
    seq_t res = 1;
    int prev = 1;                    /* previous ternary digit */

    /* Process remaining bits from the second most significant down to the least. */
    int bits = (int)w - 2;
    if (bits >= 0) {
        for (int j = bits; j >= 0; --j) {
            int bit = (n >> j) & 1;
            int d;                  /* current ternary digit */

            /* Choose the digit based on the previous digit and current binary bit. */
            if (prev == 1)              d = (bit == 0 ? -1 : 0);
            else if (prev == 0)         d = (bit == 0 ? -1 : 1);
            else /* prev == -1 */        d = (bit == 0 ? 0  : 1);

            /* Update result: res = res * 3 + d, avoiding negative additions on unsigned types. */
            if (d == -1)
                res = res * 3 - 1;
            else if (d == 0)
                res = res * 3;
            else /* d == 1 */
                res = res * 3 + 1;

            prev = d;
        }
    }

    return res;
}
