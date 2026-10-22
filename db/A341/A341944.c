/*
 * A341944  Next larger integer with same number of runs in binary expansion
 *          as n.
 * https://oeis.org/A341944
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the smallest integer > n whose binary representation has
 *            the same number of runs (consecutive equal bits) as n. This
 *            implements the definition of OEIS A341944
 * Range:     u8 n <= 169, u16 n <= 8191, u32 n <= 8191, u64 n <= 8191,
 *            u128 n <= 8191, big any n
 * Helpers:   count_runs_idx, count_runs_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/count_runs_idx.h"
#include "../utils/count_runs_seq.h"

seq_t a(idx_t n) {
    if (n == 0)          /* safety for out‑of‑range indices */
        return 1;

    size_t target = count_runs_idx(n);
    seq_t y = (seq_t)n + (seq_t)1;            /* candidate > n */

    while (count_runs_seq(y) != target)
        ++y;

    return y;
}
