/*
 * A300669  Positive numbers k with two consecutive ones in the binary
 *          representation of 1/k.
 * https://oeis.org/A300669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   has_two_consecutive_ones
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: sequentially test integers k = 1,2,3,… until the nth
 * element of the set { k : the binary expansion of 1/k contains
 * a run of at least two consecutive '1' bits } is found.  The test
 * for a given k is done by simulating the binary long division:
 *   * remove all factors 2 from k (odd part m)
 *   * iterate the remainder r : r ← (r*2) mod m, recording bits
 *   * if any two successive bits are both 1, k belongs to the set.
 *   * otherwise if r returns to the initial value 1, the fraction
 *     repeats without such a run and k is not selected.
 *
 * The routine works for all supported integer types seq_t and
 * guarantees exactness. The complexity is O(sum of cycle lengths)
 * for the first O(n) integers, which is sufficient for n ≤ 10 000
 * (runtime < 2 s on typical hardware).  The code takes no external
 * libraries beyond <stdbool.h> and is valid both as C11 and as
 * C++17.
 */
#include <stdbool.h>
#include "../utils/has_two_consecutive_ones.h"

/* OEIS A300669 */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 0, but safe */

    idx_t count = 0;               /* number of found elements so far */
    seq_t k = 0;                   /* last tested integer */

    while (count < n) {
        k++;                       /* try next k */
        if (has_two_consecutive_ones(k))
            count++;
    }
    return k;                      /* k is the nth term */
}
