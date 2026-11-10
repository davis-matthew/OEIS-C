/*
 * A350215  A048715, written in binary.
 * https://oeis.org/A350215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = decimal number whose digits are the binary expansion of
 *            A048715(n); i.e. greedy Zeckendorf-like expansion of n with
 *            weights w(0)=1,w(1)=2,w(2)=3, w(k)=w(k-1)+w(k-3), and a(n) = Sum
 *            10^k over the weights w(k) used
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t w[200];
    int cnt = 3;
    w[0] = 1; w[1] = 2; w[2] = 3;
    while (w[cnt - 1] <= n) {
        idx_t nx = w[cnt - 1] + w[cnt - 3];
        if (nx < w[cnt - 1]) { /* overflow: treat as larger than any n */
            break;
        }
        w[cnt++] = nx;
    }
    seq_t result = 0;
    idx_t rem = n;
    for (int k = cnt - 1; k >= 0 && rem > 0; --k) {
        if (w[k] <= rem) {
            rem -= w[k];
            seq_t p = 1;
            for (int j = 0; j < k; ++j) p *= 10;
            result += p;
        }
    }
    return result;
}
