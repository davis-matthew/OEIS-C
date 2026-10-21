/*
 * A286665  {0->01}-transform of the Pell word, A171588.
 * https://oeis.org/A286665
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*alpha+rho) - floor((n-1)*alpha+rho), where alpha
 *            = 2 - sqrt(2) and rho = 3 - 2*sqrt(2)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 9224, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: a(n) = 1  if floor_sqrt(2*(n+2)^2) - floor_sqrt(2*(n+1)^2) == 1
   otherwise a(n) = 0. */

#include <stdint.h>
#include <stdbool.h>

/* Integer square‑root: returns floor(sqrt(x)).  Works for any
   seq_t value up to the limits we use here. */
static inline seq_t int_sqrt(seq_t x)
{
    seq_t lo = 0, hi = x, ans = 0;
    while (lo <= hi) {
        seq_t mid = lo + ((hi - lo) >> 1);
        seq_t sq = mid * mid;
        if (sq <= x) {
            ans = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return ans;
}

seq_t a(idx_t n)
{
    /* n >= 1; treat n as seq_t */
    seq_t N = (seq_t)n;
    seq_t d1 = N + 2;          /* n+2  */
    seq_t d0 = N + 1;          /* n+1  */
    seq_t t1 = 2 * d1 * d1;    /* 2*(n+2)^2 */
    seq_t t0 = 2 * d0 * d0;    /* 2*(n+1)^2 */

    seq_t s1 = int_sqrt(t1);
    seq_t s0 = int_sqrt(t0);
    seq_t diff = s1 - s0;      /* should be 1 or 2 */

    return diff == 1 ? (seq_t)1 : (seq_t)0;
}
