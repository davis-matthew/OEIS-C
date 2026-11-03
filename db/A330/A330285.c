/*
 * A330285  The maximum number of arithmetic progressions in a sequence of
 *          length n.
 * https://oeis.org/A330285
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n} Sum_{j=1..i} floor((i - 1)/(j + 1))
 * Range:     u8 n <= 17, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Validated: partial, pass, 53 terms through n = 53 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for standard integer types, if needed */

static seq_t sum_floor_div(idx_t m)
{
    seq_t res = 0;
    for (idx_t k = 1; k <= m; ) {
        idx_t v    = m / k;          /* floor(m / k) */
        idx_t next = m / v + 1;      /* first k where floor changes */
        res += (seq_t)v * (seq_t)(next - k);
        k = next;
    }
    return res;
}

seq_t a(idx_t n)
{
    seq_t result = 0;
    if (n < 2) {
        return 0;                     /* a(1) = 0 */
    }
    for (idx_t i = 2; i <= n; ++i) {
        idx_t m = i - 1;                    /* i-1 */
        seq_t s = sum_floor_div(m);         /* sum_{k=1}^{m} floor(m/k) */
        result += s - (seq_t)m;             /* subtract floor(m/1) = m */
    }
    return result;
}
