/*
 * A327107  BII-numbers of set-systems with minimum vertex-degree > 1.
 * https://oeis.org/A327107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   ok_for
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: a(n) = BII‑number of the n‑th set‑system whose minimum
   vertex‑degree is > 1.  The sequence is obtained by testing every
   integer for the property described above. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "../utils/ok_for.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n = 0 */

    idx_t count = 0;
    uint64_t cand = 1;
    while (1) {
        if (ok_for(cand)) {
            ++count;
            if (count == n) return (seq_t)cand;
        }
        ++cand;
    }
}
