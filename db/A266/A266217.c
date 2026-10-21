/*
 * A266217  Binary representation of the n-th iteration of the "Rule 7"
 *          elementary cellular automaton starting with a single ON (black)
 *          cell.
 * https://oeis.org/A266217
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10001*a(n-2) - 10000*a(n-4) for n>5, with a(0)=1,
 *            a(1)=110, a(2)=0, a(3)=1111111, a(4)=0, a(5)=11111111111
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

typedef uint64_t idx_t; /* Example; actual type provided by build */

extern seq_t a(idx_t n);

seq_t a(idx_t n)
{
    /* handle the initial known terms */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)110;
    if (n == 2) return (seq_t)0;
    if (n == 3) return (seq_t)1111111;
    if (n == 4) return (seq_t)0;
    if (n == 5) return (seq_t)11111111111;

    /* buffer for the last 6 computed terms */
    seq_t buf[6];
    buf[0] = (seq_t)1;
    buf[1] = (seq_t)110;
    buf[2] = (seq_t)0;
    buf[3] = (seq_t)1111111;
    buf[4] = (seq_t)0;
    buf[5] = (seq_t)11111111111;

    /* compute successive terms using the recurrence */
    for (idx_t i = 6; i <= n; ++i)
    {
        idx_t i2 = (i - 2) % 6;
        idx_t i4 = (i - 4) % 6;
        idx_t im = i % 6;
        buf[im] = (seq_t)10001 * buf[i2] - (seq_t)10000 * buf[i4];
    }

    return buf[n % 6];
}
