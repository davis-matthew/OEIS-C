/*
 * A308074  Number of integer-sided triangles with perimeter n whose side
 *          lengths are mutually coprime.
 * https://oeis.org/A308074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of unordered integer‑sided triangles with perimeter
 *            n whose side lengths are pairwise coprime
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n < 3) return 0;
    seq_t cnt = 0;
    for (idx_t a = 1; a <= n / 3; ++a) {
        idx_t maxb = (n - a) / 2;          /* b ≤ c */
        for (idx_t b = a; b <= maxb; ++b) {
            idx_t c = n - a - b;           /* a + b + c = n */
            if (a + b <= c) continue;      /* triangle inequality, safety check */
            if (gcd((seq_t)a, (seq_t)b) != 1) continue;
            if (gcd((seq_t)a, (seq_t)c) != 1) continue;
            if (gcd((seq_t)b, (seq_t)c) != 1) continue;
            ++cnt;
        }
    }
    return cnt;
}
