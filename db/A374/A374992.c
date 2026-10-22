/*
 * A374992  Total cost when the elements of the n-th composition (in standard
 *          order) are requested from a self-organizing list initialized to
 *          (1, 2, 3, ...), using the move-to-front updating strategy.
 * https://oeis.org/A374992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = cost of the n‑th composition (standard order) (sum
 *            increasing, within each sum: reverse lexicographic order on the
 *            parts)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   _cost_of_composition, _generate_compositions_rev
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_cost_of_composition.h"
#include "../utils/_generate_compositions_rev.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* empty composition */

    idx_t parts[32];                 /* buffer for a composition */
    idx_t counter = 0;
    seq_t result = 0;

    for (idx_t sum = 1; ; ++sum) {
        if (_generate_compositions_rev(sum, 0, parts,
                                      n, &counter, &result))
            return result;
    }
}
