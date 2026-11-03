/*
 * A339376  Number of partitions of n into an odd number of distinct
 *          triangular numbers.
 * https://oeis.org/A339376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into an odd number of *
 *            distinct triangular numbers
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Validated: partial, pass, 91 terms through n = 90 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Return the n‑th term of OEIS A339376. */
seq_t a(idx_t n)
{
    /* Allocate DP tables for even‑size and odd‑size subsets. */
    seq_t *even = mem_new(seq_t, (size_t)(n + 1));
    seq_t *odd  = mem_new(seq_t, (size_t)(n + 1));
    if (!even || !odd) {
        mem_free(even);
        mem_free(odd);
        return (seq_t)0;
    }

    /* The empty subset has even size. */
    even[0] = (seq_t)1;
    odd[0]  = (seq_t)0;

    /* Iterate triangular numbers T_k = k(k+1)/2 up to n. */
    for (idx_t k = 1;; ++k) {
        /* Use unsigned __int128 to avoid overflow for large k. */
        idx_t t = (idx_t)(((unsigned __int128)k * (k + 1)) / 2);
        if (t > n) break;

        /* Update DP tables in reverse order to keep each triangular
         * number used at most once.  The loop stops when s == t to
         * avoid unsigned underflow. */
        for (idx_t s = n; ; --s) {
            seq_t add_even = even[s - t];
            seq_t add_odd  = odd[s - t];

            odd[s]  += add_even;
            even[s] += add_odd;

            if (s == t) break;
        }
    }

    seq_t result = odd[n];

    mem_free(even);
    mem_free(odd);
    return result;
}
