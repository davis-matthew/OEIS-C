/*
 * A315075  Coordination sequence Gal.6.350.2 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A315075
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*n + f, where f depends on (n-6) mod 6: * f = 0 if (n-6)
 *            mod 6 == 0 * f = 0 if (n-6) mod 6 == 1 * f = -1 if (n-6) mod 6
 *            == 2 * f = 0 if (n-6) mod 6 == 3 * f = +1 if (n-6) mod 6 == 4 *
 *            f = 0 if (n-6) mod 6 == 5 * For n < 6 the values are defined
 *            explicitly: * a(0)=1, a(1)=5, a(2)=9, a(3)=1...
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static const seq_t init_values[6] = {1, 5, 9, 15, 21, 25};

seq_t a(idx_t n) {
    if (n < 6) {
        return init_values[n];
    }
    seq_t val = 5 * (seq_t)n;          /* base value */
    idx_t r = (n - 6) % 6;             /* determine adjustment */
    if (r == 2) {
        val -= 1;                     /* f = -1 */
    } else if (r == 4) {
        val += 1;                     /* f = +1 */
    }                                     /* otherwise f = 0 */
    return val;
}
