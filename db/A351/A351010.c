/*
 * A351010  Numbers k such that the k-th composition in standard order is a
 *          concatenation of twins (x,x).
 * https://oeis.org/A351010
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th non‑negative integer k (starting at k=0) such
 *            that the composition obtained from k in standard order is a
 *            concatenation of twin blocks (x,x). a(1)=0. The function
 *            enumerates k in natural order and selects those whose
 *            composition satisfies the twin‑block property
 * Range:     u8 n <= 16, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   is_twin
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_twin.h"

seq_t a(idx_t n) {
    if (n == 1u) /* offset 1: first term is 0 */
        return 0;

    idx_t found = 0;                /* count of twins found after 0 */
    for (idx_t k = 1; ; ++k) {      /* iterate over k = 1, 2, 3, ... */
        if (is_twin(k)) {
            ++found;
            if (found == n - 1)     /* n‑1 twins after the initial 0 */
                return (seq_t)k;
        }
    }
}
