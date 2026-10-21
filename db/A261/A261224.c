/*
 * A261224  a(n) = number of steps needed to reach (n^2)-1 when starting from
 *          k = ((n+1)^2)-1 and repeatedly applying the map that replaces k
 *          with k - A053610(k), where A053610(k) = the number of positive
 *          squares that sum to k using the greedy algorithm.
 * https://oeis.org/A261224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps to reduce ((n+1)^2)-1 to (n^2)-1 using k
 *            → k – A053610(k) where A053610(k) is the number of squares in
 *            the greedy decomposition of k
 * Range:     u8 n <= 15, u16 n <= 338, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* smallest integer r with r*r <= x, x <= 2^64-1 */
static uint64_t isqrt64(uint64_t x)
{
    uint64_t lo = 0, hi = x;
    while (lo <= hi) {
        uint64_t mid = lo + ((hi - lo) >> 1);
        __uint128_t sq = (__uint128_t)mid * mid;
        if (sq == x)
            return mid;
        if (sq < x)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return hi;                   /* floor(sqrt(x)) */
}

/* A053610(k): number of squares in greedy decomposition of k */
static seq_t greedy_square_count(seq_t k)
{
    seq_t cnt = 0;
    uint64_t rem = (uint64_t)k;
    while (rem > 0) {
        uint64_t s = isqrt64(rem);
        rem -= s * s;
        cnt++;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t target = (seq_t)n * (seq_t)n - 1;                 /* n^2 - 1   */
    seq_t start  = (seq_t)(n + 1) * (seq_t)(n + 1) - 1;     /* (n+1)^2-1 */
    seq_t k = start;
    seq_t steps = 0;

    while (k > target) {
        seq_t sub = greedy_square_count(k);   /* A053610(k)      */
        k -= sub;
        steps++;
    }
    return steps;
}
