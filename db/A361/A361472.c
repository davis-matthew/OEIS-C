/*
 * A361472  Size of the symmetric differences of {1,2,3,4,5}, {2,4,6,8,10},
 *          ..., {n,2n,3n,4n,5n}.
 * https://oeis.org/A361472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 127, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = # of integers m (1 ≤ m ≤ 5n) having an odd
 * number of representations m = k·t with k ≤ n and t ∈ {1,2,3,4,5}.
 *
 * This counts the size of the symmetric difference of the sets
 * {k,2k,3k,4k,5k} for k = 1..n.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset 1; not used but safe     */
    seq_t count = 0;
    idx_t limit = n * 5;           /* 5*n fits in idx_t for n≤10000   */

    for (idx_t m = 1; m <= limit; ++m) {
        unsigned occ = 0;

        /* t = 1 : always a divisor, k = m */
        if (m <= n) occ++;

        /* t = 2 */
        if ((m & 1) == 0) {        /* m % 2 == 0 */
            if (m / 2 <= n) occ++;
        }

        /* t = 3 */
        if (m % 3 == 0) {
            if (m / 3 <= n) occ++;
        }

        /* t = 4 */
        if ((m & 3) == 0) {        /* m % 4 == 0 */
            if (m / 4 <= n) occ++;
        }

        /* t = 5 */
        if (m % 5 == 0) {
            if (m / 5 <= n) occ++;
        }

        if (occ & 1u) count++;
    }

    return count;
}
