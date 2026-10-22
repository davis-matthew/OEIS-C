/*
 * A355498  a(n) = A000217(A033676(n)) * A000217(A033677(n)).
 * https://oeis.org/A355498
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000217(A033676(n)) * A000217(A033677(n)) where
 *            A033676(n) is the largest divisor of n not exceeding √n,
 *            A033677(n) = n / A033676(n), and A000217(m) = m*(m+1)/2
 * Range:     u8 n <= 22, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static inline idx_t isqrt(idx_t n)
{
    /* integer square root (floor) via binary search, safe for 64‑bit n */
    idx_t low = 0, high = (n >> 1) + 1, mid, res = 0;
    while (low <= high) {
        mid = (low + high) >> 1;
        unsigned __int128 sq = (unsigned __int128)mid * (unsigned __int128)mid;
        if (sq == n)
            return mid;
        if (sq < n) {
            res = mid;
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return res;
}

static inline seq_t triangular(idx_t m)
{
    return ((seq_t)m * ((seq_t)m + 1)) / 2;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                /* undefined for n = 0, defensive */

    idx_t sqrt_n = isqrt(n);
    idx_t a_div = 1;              /* fallback divisor */

    /* Find the largest divisor ≤ √n */
    for (idx_t d = sqrt_n; d >= 1; --d) {
        if (n % d == 0) {
            a_div = d;
            break;
        }
    }

    idx_t b_div = n / a_div;

    seq_t t_a = triangular(a_div);
    seq_t t_b = triangular(b_div);

    return t_a * t_b;
}
