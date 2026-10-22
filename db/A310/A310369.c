/*
 * A310369  Coordination sequence Gal.5.64.5 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-9) - a(n-10) for n > 10 a(0) = 1, a(1) = 4,
 *            a(2) = 10, a(3) = 14, a(4) = 18, a(5) = 22, a(6) = 26, a(7) =
 *            30, a(8) = 36, a(9) = 40, a(10) = 44
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Recursive definition using a circular buffer of size 11.
   This works for any seq_t that supports +, -, and assignments,
   and for any idx_t that can represent the index up to the
   desired maximum (typically uint64_t).                               */
seq_t a(idx_t n)
{
    static const seq_t init[11] = {
        (seq_t)1,  (seq_t)4,  (seq_t)10, (seq_t)14, (seq_t)18,
        (seq_t)22, (seq_t)26, (seq_t)30, (seq_t)36, (seq_t)40,
        (seq_t)44
    };

    if (n <= 10)
        return init[(size_t)n];

    seq_t buf[11];
    for (size_t i = 0; i <= 10; ++i)
        buf[i % 11] = init[i];

    for (idx_t i = 11; i <= n; ++i) {
        size_t im1 = (size_t)(i - 1) % 11;
        size_t im9 = (size_t)(i - 9) % 11;
        size_t im10 = (size_t)(i - 10) % 11;
        seq_t val = buf[im1] + buf[im9] - buf[im10];
        buf[i % 11] = val;
    }

    return buf[n % 11];
}
