/*
 * A298801  Fourth column of triangular array in A296339.
 * https://oeis.org/A298801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) - a(n-4) for n ≥ 12 * Initial values (n =
 *            0 … 11): * a(0)=4, a(1)=3, a(2)=6, a(3)=0, a(4)=1, a(5)=2, *
 *            a(6)=5, a(7)=9, a(8)=12, a(9)=7, a(10)=8, a(11)=15 * * The
 *            implementation iteratively evaluates the recurrence * without
 *            allocating dynamic memory; it works for any * seq_...
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t base_vals[12] = {
    (seq_t)4, (seq_t)3, (seq_t)6, (seq_t)0,
    (seq_t)1, (seq_t)2, (seq_t)5, (seq_t)9,
    (seq_t)12, (seq_t)7, (seq_t)8, (seq_t)15
};

seq_t a(idx_t n)
{
    if (n < 12u) {
        return base_vals[n];
    }

    /* buffer holds the last 4 computed values:
     * buf[0] = a(k-4), buf[1] = a(k-3), buf[2] = a(k-2), buf[3] = a(k-1)
     */
    seq_t buf[4];
    buf[0] = base_vals[8];  /* a(8)  */
    buf[1] = base_vals[9];  /* a(9)  */
    buf[2] = base_vals[10]; /* a(10) */
    buf[3] = base_vals[11]; /* a(11) */

    for (idx_t k = 12; k <= n; ++k) {
        seq_t val = buf[3] + buf[1] - buf[0];   /* a(k-1) + a(k-3) - a(k-4) */
        buf[0] = buf[1];
        buf[1] = buf[2];
        buf[2] = buf[3];
        buf[3] = val;
    }

    return buf[3];   /* a(n) */
}
