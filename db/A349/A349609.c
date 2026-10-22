/*
 * A349609  Number of solutions to x^2 + y^2 <= n^2, where x, y are positive
 *          odd integers.
 * https://oeis.org/A349609
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions to x^2 + y^2 <= n^2 with x, y
 *            positive odd integers
 * Range:     u8 n <= 36, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>

/* Integer square root of a 128‑bit unsigned value.
   Returns floor(sqrt(v)). */
static inline uint64_t isqrt_u128(unsigned __int128 v)
{
    uint64_t low = 0, high = ULLONG_MAX, mid, ans = 0;
    while (low <= high) {
        mid = low + (high - low) / 2;
        unsigned __int128 sq = (unsigned __int128)mid * mid;
        if (sq <= v) {
            ans = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;
            high = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* Convert index to native unsigned integer for loops. */
    uint64_t n64 = (uint64_t)n;
    unsigned __int128 n_sq = (unsigned __int128)n64 * n64;  /* n^2 */

    seq_t total = 0;

    /* Iterate over positive odd x values. */
    for (uint64_t x = 1; x <= n64; x += 2) {
        unsigned __int128 diff = n_sq - (unsigned __int128)x * x; /* n^2 - x^2 */
        uint64_t m = isqrt_u128(diff);                            /* floor sqrt */

        /* Count odd y <= m: 1,3,5,... <= m. */
        seq_t cnt = ((seq_t)m + 1) / 2;
        total += cnt;
    }

    return total;
}
