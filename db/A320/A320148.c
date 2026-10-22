/*
 * A320148  Number of integer solutions to a^2 + b^2 + 3*c^2 + 6*d^2 = n.
 * https://oeis.org/A320148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer solutions to a^2 + b^2 + 3*c^2 + 6*d^2
 *            = n
 * Range:     u8 n <= 66, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Integer square root: floor(sqrt(x)) for 0 <= x <= 2^64-1 */
static inline idx_t isqrt(idx_t x)
{
    idx_t left = 0;
    idx_t right = x;
    /* Binary search on [0, x] */
    while (left < right)
    {
        idx_t mid = (left + right + 1) >> 1;   /* upper mid to avoid infinite loop */
        if (mid <= x / mid)   /* mid*mid <= x without overflow */
            left = mid;
        else
            right = mid - 1;
    }
    return left;
}

/* Test whether x is a perfect square */
static inline bool is_square(idx_t x)
{
    idx_t r = isqrt(x);
    return r * r == x;
}

/* Count ordered integer pairs (a, b) such that a^2 + b^2 = rem.
 * The count includes all sign choices for a and b. */
static seq_t count_two_squares(idx_t rem)
{
    seq_t total = 0;
    idx_t a_max = isqrt(rem);
    for (idx_t a = 0; a <= a_max; ++a)
    {
        idx_t a2 = a * a;
        idx_t b2 = rem - a2;
        if (!is_square(b2))
            continue;

        seq_t mult_a = (a == 0) ? 1 : 2;        /* signs for a */
        seq_t mult_b = (b2 == 0) ? 1 : 2;        /* signs for b */
        total += mult_a * mult_b;
    }
    return total;
}

seq_t a(idx_t n)
{
    seq_t ans = 0;

    /* iterate over d >= 0 such that 6*d^2 <= n */
    idx_t d_max = isqrt(n / 6);
    for (idx_t d = 0; d <= d_max; ++d)
    {
        seq_t mult_d = (d == 0) ? 1 : 2;        /* signs for d */
        idx_t d_term = 6 * d * d;
        if (d_term > n)
            break;

        /* iterate over c >= 0 such that 3*c^2 <= n - 6*d^2 */
        idx_t c_max = isqrt((n - d_term) / 3);
        for (idx_t c = 0; c <= c_max; ++c)
        {
            seq_t mult_c = (c == 0) ? 1 : 2;    /* signs for c */
            idx_t c_term = 3 * c * c;
            if (n < d_term + c_term)
                break;

            idx_t rem = n - d_term - c_term;
            seq_t cnt_ab = count_two_squares(rem);
            ans += cnt_ab * (mult_d * mult_c);
        }
    }

    return ans;
}
