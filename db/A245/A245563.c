/*
 * A245563  Table read by rows: row n gives list of lengths of runs of 1's in
 *          binary expansion of n, starting with low-order bits.
 * https://oeis.org/A245563
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, tabf, changed
 * Formula:   a(n) is the n‑th element of the flattened list formed by
 *            concatenating, in order of n, the lengths of runs of 1 bits in
 *            the binary representation of n, with a(0)=0
 * Range:     u8 n <= 2770, u16 n <= 2770, u32 n <= 2770, u64 n <= 2770,
 *            u128 n <= 2770, big any n
 * Helpers:   kth_run_length, run_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/kth_run_length.h"
#include "../utils/run_count.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    idx_t target = n;            /* offset in the flattened table (1‑based) */
    idx_t cum = 0;
    for (idx_t cur = 0; ; ++cur) {
        int rc = run_count(cur);
        if ((idx_t)rc + cum >= target) {
            idx_t k = target - cum;   /* 1‑based index within cur's runs */
            return kth_run_length(cur, k);
        }
        cum += rc;
    }
}
