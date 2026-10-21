/*
 * A248835  a(n) = n + A033677(n), where A033677(n) is the smallest divisor of
 *          n >= sqrt(n).
 * https://oeis.org/A248835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + smallest divisor of n that is ≥ sqrt(n) (A033677)
 * Range:     u8 n <= 62, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for unsigned 64‑bit n. */
static inline idx_t floor_isqrt_u64(idx_t n)
{
    if (n == 0) return 0;
    idx_t low = 1, high = n, best = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        if (mid <= n / mid) {          /* mid*mid ≤ n  */
            best = mid;
            low  = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return best;
}

/* a(n) = n + A033677(n), where A033677(n) = smallest divisor of n ≥ sqrt(n). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* safe guard; not used in OEIS offset 1 */
    idx_t s = floor_isqrt_u64(n);   /* largest d with d ≤ sqrt(n) that divides n */
    idx_t d = s;
    for (; d >= 1; --d) {
        if (n % d == 0) break;
    }
    /* d is now the largest divisor ≤ sqrt(n); smallest divisor ≥ sqrt(n) is n/d */
    idx_t div = n / d;
    return (seq_t)n + (seq_t)div;
}
