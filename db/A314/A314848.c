/*
 * A314848  Coordination sequence Gal.5.110.3 where Gal.u.t.v denotes the
 *          coordination sequence for a vertex of type v in tiling number t in
 *          the Galebach list of u-uniform tilings.
 * https://oeis.org/A314848
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1, a(1)=5, a(2)=9, a(3)=14; For n >= 4, a(n) = a(3) +
 *            sum_{k=4}^{n} diff_k, where diff_k follows a period‑9 pattern:
 *            diff_k = 4,6,4,5,4,5,5,4,5 (offset at k=4) i.e. diff_k =
 *            pattern[(k-4) % 9]. Thus a(n) = 14 + 42 * floor((n-3)/9) +
 *            sum_{i=0}^{(n-3)%9-1} pattern[i]
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n) {
    /* Handle the first four terms explicitly. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)14;

    /* For n >= 4, use the periodic difference pattern. */
    static const seq_t pattern[9] = {4, 6, 4, 5, 4, 5, 5, 4, 5};
    idx_t len = n - 3;                /* number of difference steps from 4 to n */
    idx_t q   = len / 9;              /* complete cycles of the 9‑term pattern    */
    idx_t r   = len % 9;              /* remaining terms of the current cycle    */

    /* Base value at n = 3 plus cycles plus partial cycle. */
    seq_t sum = (seq_t)14 + (seq_t)42 * (seq_t)q;

    for (idx_t i = 0; i < r; ++i) {
        sum += pattern[i];
    }
    return sum;
}
