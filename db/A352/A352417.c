/*
 * A352417  Inverse permutation to A352416.
 * https://oeis.org/A352417
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ternary value of the greedy (minimal) Pell representation
 *            of n, with the 0 that follows each digit 2 dropped. The weights
 *            are the Pell numbers A000129(k+1) = 1, 2, 5, 12, 29, ... This
 *            undoes A352416
 * Range:     u8 n <= 77, u16 n <= 6725, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 8957 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    if (m == 0) return (seq_t)0;

    seq_t w[256];
    size_t cnt = 1;
    w[0] = 1;
    if (m >= 2) {
        w[1] = 2;
        cnt = 2;
        for (;;) {
            seq_t p = w[cnt - 1], q = w[cnt - 2];
            /* next weight 2p+q exceeds m  <=>  p > (m-q)/2 (overflow-safe) */
            if (p > (m - q) / 2) break;
            w[cnt] = p * 2 + q;
            cnt++;
        }
    }

    seq_t result = 0;
    bool skip = false;
    for (size_t k = cnt; k-- > 0;) {
        seq_t d = m / w[k];
        m -= d * w[k];
        if (skip) { skip = false; continue; }   /* the 0 placed after a 2 */
        result = result * 3 + d;
        if (d == 2) skip = true;
    }
    return result;
}
