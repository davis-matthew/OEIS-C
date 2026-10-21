/*
 * A291036  p-INVERT of (1,0,0,1,0,0,1,0,0,...), where p(S) = 1 - 2 S - 2 S^2.
 * https://oeis.org/A291036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 41, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = 2*a(n-1) + 2*a(n-2) + 2*a(n-3) - 2*a(n-4) - a(n-6)
 * initial terms: a(0)=2, a(1)=6, a(2)=16, a(3)=46, a(4)=132, a(5)=376, a(6)=1074
 *
 * The implementation uses the recurrence directly, keeping only the last
 * seven values in a circular buffer.  It works for any seq_t (unsigned 8/16/32/64‑bit
 * or arbitrary‑precision type) because all arithmetic is performed on seq_t
 * operands, whose usual arithmetic conversions guarantee modulo‑2ⁿ behaviour
 * for the unsigned integer types used in the tests.
 */
#include <stddef.h>   /* for size_t */

static const seq_t init_vals[7] = { 2, 6, 16, 46, 132, 376, 1074 };

seq_t a(idx_t n)
{
    if (n <= 6u)
        return init_vals[(size_t)n];

    /* circular buffer holding the last 7 computed values */
    seq_t buf[7] = { 2, 6, 16, 46, 132, 376, 1074 };

    /* compute terms from 7 up to n */
    for (idx_t i = 7u; i <= n; ++i) {
        /* indices are taken modulo 7; using (i - k) % 7 works for i >= 7 */
        seq_t val = 2 * buf[(i - 1u) % 7u]
                  + 2 * buf[(i - 2u) % 7u]
                  + 2 * buf[(i - 3u) % 7u]
                  - 2 * buf[(i - 4u) % 7u]
                  - buf[(i - 6u) % 7u];
        buf[i % 7u] = val;
    }

    return buf[n % 7u];
}
