/*
 * A395438  Largest divisor of n not exceeding 1 + sqrt(1 + 8*n)/2.
 * https://oeis.org/A395438
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = largest divisor d of n such that d*(d-1)/2 <= n
 *            (equivalently, d <= ⌊(1+√(1+8n))/2⌋)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute floor of the square root of a 128‑bit unsigned integer.
   The result fits in 64 bits (≤ 2^64‑1). */
static uint64_t floor_sqrt_u128(unsigned __int128 v)
{
    uint64_t low = 0, high = (uint64_t)-1, ans = 0;
    while (low <= high) {
        uint64_t mid = low + ((high - low) >> 1);
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq == v) return mid;
        if (sq < v) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

/* Return a(n) for OEIS sequence A395438. */
seq_t a(idx_t n)
{
    /* compute the upper bound limit = floor((1+sqrt(1+8n))/2) */
    unsigned __int128 val = (unsigned __int128)1
                         + (unsigned __int128)8 * (unsigned __int128)n;
    uint64_t t = floor_sqrt_u128(val);
    uint64_t limit = (1 + t) / 2;

    /* search downward for the largest divisor ≤ limit */
    uint64_t d = limit;
    while (d >= 1) {
        if (n % d == 0)
            return (seq_t)d;
        if (d == 1)
            break;
        --d;
    }
    return (seq_t)1;   /* fallback, should never reach here for n≥1 */
}
