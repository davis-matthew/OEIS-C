/*
 * A315327  Coordination sequence Gal.6.329.6 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(0)=1, a(1)=6, a(2)=10, a(3)=16, a(4)=22, a(5)=26, a(6)=30,
 * a(7)=36, a(8)=42, a(9)=46, a(10)=52.
 * For n > 10:  a(n) = 2*a(n-1) - 2*a(n-2) + 2*a(n-3) - 2*a(n-4) + 2*a(n-5)
 *                - 2*a(n-6) + 2*a(n-7) - 2*a(n-8) + 2*a(n-9) - a(n-10).
 *
 * The implementation uses only integer arithmetic and works for any
 * unsigned seq_t type (8, 16, 32, 64 or 128 bits, or an arbitrary‑precision
 * integer class).  It has linear time complexity in n and constant
 * auxiliary storage.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* Initial terms 0 … 10.  Stored as a static array so the values are
       available without any dynamic allocation. */
    static const seq_t init[11] = {
        1ULL, 6ULL, 10ULL, 16ULL, 22ULL,
        26ULL, 30ULL, 36ULL, 42ULL, 46ULL, 52ULL
    };

    /* For the first 11 indices we can return immediately. */
    if (n < 11)
        return init[(size_t)n];

    /* Buffer holding the last 11 computed values.  Start with the
       initial terms. */
    seq_t buf[11];
    for (size_t i = 0; i < 11; ++i)
        buf[i] = init[i];

    /* Compute the sequence from n = 11 up to the required index. */
    for (idx_t i = 11; i <= n; ++i) {
        seq_t val =  2 * buf[10]
                   - 2 * buf[9]
                   + 2 * buf[8]
                   - 2 * buf[7]
                   + 2 * buf[6]
                   - 2 * buf[5]
                   + 2 * buf[4]
                   - 2 * buf[3]
                   + 2 * buf[2]
                   -     buf[1];

        /* Shift the buffer to make room for the new value. */
        for (int k = 0; k < 10; ++k)
            buf[k] = buf[k + 1];
        buf[10] = val;
    }

    return buf[10];
}
