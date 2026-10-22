/*
 * A311625  Coordination sequence Gal.5.99.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A311625
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = if n < 4: a(0)=1, a(1)=4, a(2)=8, a(3)=12 else: a(n) = 12
 *            + 36*⌊(n-3)/8⌋ + P[(n-3)%8] where P = [0,6,12,16,20,24,28,32]
 *            (P[0] corresponds to n = 3)
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static const seq_t P[9] = {
    (seq_t)0,  (seq_t)6,  (seq_t)12, (seq_t)16,
    (seq_t)20, (seq_t)24, (seq_t)28, (seq_t)32,
    (seq_t)36   /* not used directly, but convenient for bounds */
};

seq_t a(idx_t n)
{
    /* handle the first four terms explicitly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)12;

    /* n >= 4 */
    idx_t steps = n - 3;               /* number of increments from a(3) to a(n) */
    idx_t q = steps / 8;               /* number of full cycles of 8 */
    idx_t r = steps % 8;               /* remaining indices in the cycle */

    /* compute a(n) using the derived closed form */
    seq_t sum = (seq_t)36 * (seq_t)q + P[r];
    return (seq_t)12 + sum;
}
