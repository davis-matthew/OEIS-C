/*
 * A298789  Coordination sequence for bey tiling (or net) with respect to a
 *          tetravalent node.
 * https://oeis.org/A298789
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-3) - a(n-6) for n>6; a(0)=1, a(1)=4, a(2)=7,
 *            a(3)=10, a(4)=15, a(5)=18, a(6)=20
 * Range:     u8 n <= 69, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    static const seq_t init[] = {1, 4, 7, 10, 15, 18, 20};

    if (n <= 6) {
        return init[(size_t)n];
    }

    seq_t buf[7];
    for (size_t i = 0; i < 7; ++i) {
        buf[i] = init[i];
    }

    for (idx_t i = 7; i <= n; ++i) {
        seq_t v = 2 * buf[(i - 3) % 7] - buf[(i - 6) % 7];
        buf[i % 7] = v;
    }

    return buf[n % 7];
}
