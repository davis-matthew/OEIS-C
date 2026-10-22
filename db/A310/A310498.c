/*
 * A310498  Coordination sequence Gal.3.20.1 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A310498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1 * for n > 0: a(n) = 5*n + δ, where * δ = 0 if n mod 4
 *            == 0 or 2 * = -1 if n mod 4 == 1 * = +1 if n mod 4 == 3 * This
 *            satisfies the initial terms: * a(0)=1, a(1)=4, a(2)=10, a(3)=16,
 *            a(4)=20, a(5)=24, …
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* compute 5*n in seq_t */
    seq_t five_n = (seq_t)n * (seq_t)5;
    /* determine n mod 4, safe because n is unsigned */
    unsigned rem = (unsigned)(n % 4);

    switch (rem) {
        case 0: /* n mod 4 == 0 -> δ = 0 */
        case 2: /* n mod 4 == 2 -> δ = 0 */
            return five_n;
        case 1: /* n mod 4 == 1 -> δ = -1 */
            return five_n - (seq_t)1;
        case 3: /* n mod 4 == 3 -> δ = +1 */
            return five_n + (seq_t)1;
        default:
            /* unreachable */
            return five_n;
    }
}
