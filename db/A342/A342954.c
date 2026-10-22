/*
 * A342954  Number of interior regions formed by drawing n unit circles whose
 *          centers (s,t) are the partitions of k into 2 parts, where 0 < s <=
 *          t and s + t = k, ordered by increasing values of s and where k =
 *          2,3,... .
 * https://oeis.org/A342954
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=1; for n>1, a(n) = Sum_{i=1}^{n-1} (7
 *            -4*(floor(sqrt(i))-floor(sqrt(i-1))
 *            +round(sqrt(i+1))-round(sqrt(i)))
 *            -2*(floor(sqrt(i+1))-floor(sqrt(i)))
 * Range:     u8 n <= 45, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root: floor(sqrt(n)) for unsigned 64‑bit n. */
static inline idx_t int_sqrt_idx(idx_t n)
{
    idx_t low = 0, high = n + 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= n / mid)
            low = mid;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    if (n == 1)
        return 1;                 /* a(1) */
    seq_t acc = 0;
    for (idx_t i = 1; i < n; ++i) {
        /* floor sqrt and round for i  */
        idx_t s      = int_sqrt_idx(i);
        idx_t t      = i - s * s;
        idx_t round_i = s + ((t >= s + 1) ? 1 : 0);

        /* floor sqrt and round for i+1  */
        idx_t s_next      = int_sqrt_idx(i + 1);
        idx_t t_next      = (i + 1) - s_next * s_next;
        idx_t round_ip1 = s_next + ((t_next >= s_next + 1) ? 1 : 0);

        /* indicators */
        int diff_floor      = (s * s == i)          ? 1 : 0;  /* i perfect square   */
        int diff_floor_next = (s_next * s_next == i + 1)? 1 : 0;/* i+1 perfect square  */
        int diff_round      = (int)(round_ip1 - round_i);    /* 0 or 1             */

        seq_t f = 7
                  - 4 * (seq_t)(diff_floor + diff_round)
                  - 2 * (seq_t)diff_floor_next;

        acc += f;
    }
    return acc;
}
