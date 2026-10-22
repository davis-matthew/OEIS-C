/*
 * A308209  Number of integer-sided triangles with perimeter n whose largest
 *          side length is coprime to n.
 * https://oeis.org/A308209
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer‑sided triangles with perimeter n whose
 *            largest side length is coprime to n
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    if (n < 3) return 0;

    seq_t result = 0;

    idx_t lower = (n + 2) / 3;          /* ceil(n/3) */
    idx_t upper = (n - 1) / 2;          /* floor((n-1)/2) */

    for (idx_t c = lower; c <= upper; ++c) {
        idx_t L = n - 2 * c;           /* lower bound on a */
        idx_t U = (n - c) / 2;         /* upper bound on a */

        if (U < L) continue;           /* no valid (a,b) pairs */

        idx_t cnt = U - L + 1;          /* number of (a,b) with a<=b<=c */

        if (gcd((seq_t)c, (seq_t)n) == (seq_t)1)
            result += (seq_t)cnt;
    }

    return result;
}
