/*
 * A284569  a(n) = LCM of the lengths of runs of 1-bits in binary
 *          representation of n.
 * https://oeis.org/A284569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = LCM of the lengths of runs of 1‑bits in the binary
 *            representation of n
 * Range:     u8 n <= 9109, u16 n <= 9109, u32 n <= 9109, u64 n <= 9109,
 *            u128 n <= 9109, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    idx_t x = n;
    seq_t lcm = (seq_t)1;

    while (x) {
        /* Count consecutive 1s */
        if (x & (idx_t)1) {
            idx_t run = 0;
            while (x & (idx_t)1) {
                run++;
                x >>= 1;
            }
            seq_t r = (seq_t)run;
            seq_t g = gcd(lcm, r);
            lcm = (lcm / g) * r;
        }
        /* Skip zeros until next 1 or until x becomes zero */
        while (x && !(x & (idx_t)1))
            x >>= 1;
    }

    return lcm;
}
