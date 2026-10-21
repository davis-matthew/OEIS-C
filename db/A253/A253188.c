/*
 * A253188  Minimal positive integer k such that n^n >= (n-k)^(n+k).
 * https://oeis.org/A253188
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = min{ k>=1 | n^n >= (n-k)^(n+k) } (binary search on k)
 * Range:     u8 n <= 273, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n <= 1u) {
        return (seq_t)1;
    }

    /* Pre‑compute log(n) as long double for the right side. */
    const long double log_n = (long double)n * logl((long double)n);

    idx_t low  = 1;
    idx_t high = n - 1;          /* k must satisfy 1 <= k <= n-1 (n-k > 0). */

    while (low < high) {
        idx_t mid = low + (high - low) / 2;

        /* log of left side: (n+mid) * log(n-mid). */
        const long double log_left =
            ((long double)(n + mid)) *
            logl((long double)(n - mid));

        /* boolean holding condition. */
        if (log_n >= log_left) {
            /* inequality holds: try smaller k. */
            high = mid;
        } else {
            /* inequality fails: need larger k. */
            low = mid + 1;
        }
    }

    return (seq_t)low;
}
