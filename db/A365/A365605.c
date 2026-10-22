/*
 * A365605  Characteristic function of numbers without an inferior odd divisor
 *          > 1.
 * https://oeis.org/A365605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 iff n has no odd divisor d>1 with d*d <= n, otherwise 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Integer square‑root of an unsigned 64‑bit integer, floor. */
static inline idx_t floor_sqrt_u64(idx_t n)
{
    if (n < 2) return n;           /* √0 = 0, √1 = 1 */
    idx_t low  = 1;
    idx_t high = n >> 1;           /* high is at least √n */
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq  = mid * mid;
        if (sq == n) return mid;
        if (sq < n) {
            low = mid + 1;         /* need a larger guess */
        } else {
            high = mid - 1;        /* mid was too big */
        }
    }
    return high;                    /* floor(√n) */
}

seq_t a(idx_t n)
{
    /* a(1) = 1 by definition of the offset; a(0) is unused. */
    if (n < 2) return (seq_t)1;

    idx_t lim = floor_sqrt_u64(n);
    /* If the square root is < 3, no odd divisor > 1 can be <= sqrt(n). */
    if (lim < 3) return (seq_t)1;

    for (idx_t d = 3; d <= lim; d += 2) {
        if (n % d == 0) return (seq_t)0;   /* odd divisor ≤ sqrt(n) found */
    }
    return (seq_t)1;                        /* no such divisor exists */
}
