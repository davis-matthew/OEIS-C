/*
 * A249732  Number of (not necessarily distinct) multiples of 4 on row n of
 *          Pascal's triangle.
 * https://oeis.org/A249732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ k : 0 ≤ k ≤ n , v₂(C(n,k)) ≥ 2 }| where v₂(C(n,k)) =
 *            popcount(k) + popcount(n‑k) – popcount(n)
 * Range:     u8 n <= 263, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* popcount for 64‑bit values (portable implementation). */
static inline int popcnt_u64(uint64_t x)
{
    int c = 0;
    while (x) {
        x &= x - 1;   /* drop lowest set bit */
        ++c;
    }
    return c;
}

seq_t a(idx_t n)
{
    int ones_n = popcnt_u64(n);
    seq_t count = 0;
    for (idx_t k = 0; k <= n; ++k) {
        int v2 = popcnt_u64(k) + popcnt_u64(n - k) - ones_n;
        if (v2 >= 2) {
            ++count;
        }
    }
    return count;
}
