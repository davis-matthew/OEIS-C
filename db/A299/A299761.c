/*
 * A299761  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          row n lists the middle divisors of n, or 0 if there are no middle
 *          divisors of n.
 * https://oeis.org/A299761
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, look
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 1206, u32 n <= 8333, u64 n <= 8333,
 *            u128 n <= 8333, big any n
 * Helpers:   get_middle_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: a(n) is the n-th element of the irregular triangle
   formed by the middle divisors of each integer m (or 0 if none).
   The triangle is read by rows, and the sequence enumerates those
   rows.  The function computes the value by iterating through
   rows until the desired index is found, then returns the
   corresponding middle divisor (or 0).  No memoization is used,
   which is sufficient for n up to about 10000. */

#include <stddef.h>
#include "../utils/get_middle_divisors.h"

static void sort_seq_t(seq_t *arr, size_t len)
{
    for (size_t i = 1; i < len; ++i) {
        seq_t key = arr[i];
        size_t j = i;
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            --j;
        }
        arr[j] = key;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined for n < 1, return 0 as guard */

    idx_t current_index = 0;          /* number of elements already output */
    for (idx_t m = 1; ; ++m) {
        seq_t mids[128];
        size_t mid_count = get_middle_divisors(m, mids);

        if (mid_count == 0) {         /* row contains a single 0 */
            if (current_index + 1 >= n) return 0;
            current_index += 1;
            continue;
        }

        sort_seq_t(mids, mid_count);

        if (current_index + mid_count >= n) {
            size_t offset = (size_t)(n - current_index - 1); /* 0‑based */
            return mids[offset];
        }

        current_index += mid_count;
    }
}
