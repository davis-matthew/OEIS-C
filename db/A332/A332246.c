/*
 * A332246  a(n) is the X-coordinate of the n-th point of the Minkowski
 *          sausage (or Minkowski curve). Sequence A332247 gives
 *          Y-coordinates.
 * https://oeis.org/A332246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{k=0}^{n-1} δ(t(k) mod 4), where δ(0)=1, δ(2)=–1, else
 *            0. t(k) = (# of digits 1 or 6) – (# of digits 3 or 4) in base‑8
 *            representation of k
 * Range:     u8 n <= 4095, u16 n <= 4096, u32 n <= 4096, u64 n <= 4096,
 *            u128 n <= 4096, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t k = 0; k < n; ++k) {
        int t = 0;
        idx_t x = k;
        while (x != 0) {
            unsigned d = (unsigned)(x & 7u);          /* base‑8 digit */
            if (d == 1u || d == 6u) ++t;
            else if (d == 3u || d == 4u) --t;
            x >>= 3;
        }
        int r = t % 4;
        if (r < 0) r += 4;
        if (r == 0)
            sum += (seq_t)1;
        else if (r == 2)
            sum -= (seq_t)1;
    }
    return sum;
}
