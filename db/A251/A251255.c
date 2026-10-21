/*
 * A251255  Number of (n+1)X(5+1) 0..1 arrays with every 2X2 subblock having a
 *          single 1 or two 1s on the same edge.
 * https://oeis.org/A251255
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   the sequence counts (n+1)×6 binary arrays with the * constraint
 *            that every 2×2 subblock has either exactly one 1 or * two 1s on
 *            the same edge. This is computed by dynamic programming * over
 *            6‑bit rows. Allowed 2×2 patterns (tl,tr,bl,br) are the 8 *
 *            binary words with 1 or 2 adjacent 1s: * 1000,010...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 23, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    static bool allowed[64][64] = {0};
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        for (int a = 0; a < 64; ++a) {
            for (int b = 0; b < 64; ++b) {
                bool ok = true;
                for (int c = 0; c < 5; ++c) {
                    int tl = (a >> c) & 1;
                    int tr = (a >> (c + 1)) & 1;
                    int bl = (b >> c) & 1;
                    int br = (b >> (c + 1)) & 1;
                    int p = (tl << 3) | (tr << 2) | (bl << 1) | br;
                    if (!(p == 8 || p == 4 || p == 2 || p == 1 ||
                          p == 12 || p == 3 || p == 10 || p == 5))
                    {
                        ok = false;
                        break;
                    }
                }
                allowed[a][b] = ok;
            }
        }
    }

    seq_t dp_cur[64], dp_next[64];
    const seq_t zero = 0;
    const seq_t one  = 1;

    for (int i = 0; i < 64; ++i) dp_cur[i] = one;

    for (idx_t step = 0; step < n; ++step) {
        for (int i = 0; i < 64; ++i) dp_next[i] = zero;
        for (int a = 0; a < 64; ++a) {
            seq_t v = dp_cur[a];
            if (v == zero) continue;
            for (int b = 0; b < 64; ++b)
                if (allowed[a][b]) {
                    dp_next[b] += v;
                }
        }
        for (int i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
    }

    seq_t total = zero;
    for (int i = 0; i < 64; ++i) total += dp_cur[i];
    return total;
}
