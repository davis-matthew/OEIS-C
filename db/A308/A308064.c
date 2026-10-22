/*
 * A308064  Number of triangles with perimeter n whose side lengths are square
 *          numbers.
 * https://oeis.org/A308064
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (i,j,k) | 1 ≤ i ≤ j ≤ k, i² + j² + k² = n and i² + j²
 *            > k² }|
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline idx_t isqrt(idx_t n)
{
    idx_t low = 0;
    idx_t high = n;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid > 0 && mid > n / mid) {
            high = mid - 1;
        } else {
            if (mid + 1 > n / (mid + 1)) {
                return mid;
            }
            low = mid + 1;
        }
    }
    return high;
}

seq_t a(idx_t n)
{
    if (n < 3) return 0;
    seq_t count = 0;
    idx_t max_x = isqrt(n);

    for (idx_t x = 1; x <= max_x; ++x) {
        idx_t sx = x * x;
        idx_t rem_after_x = n - sx;
        if (rem_after_x < 2) continue;          /* need at least two squares left */

        idx_t max_y = isqrt(rem_after_x);
        for (idx_t y = x; y <= max_y; ++y) {
            idx_t sy = y * y;
            idx_t sum2 = sx + sy;
            if (sum2 > n) break;

            idx_t rem = n - sum2;
            if (rem == 0) continue;            /* zero side not allowed */

            idx_t z = isqrt(rem);
            if ((idx_t)z * z != rem) continue; /* not a perfect square */
            if ((idx_t)z < y) continue;        /* maintain ordering i ≤ j ≤ k */

            if (sum2 <= rem) continue;         /* triangle inequality */
            ++count;
        }
    }
    return count;
}
