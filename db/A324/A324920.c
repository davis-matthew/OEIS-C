/*
 * A324920  a(n) is the number of iterations of the integer splitting function
 *          (A056737) necessary to reach zero.
 * https://oeis.org/A324920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 0; a(n) = 1 + a(A056737(n)) for n >= 1, where A056737(n)
 *            = n/d - d with d the largest divisor of n with d <= sqrt(n).
 *            Computed by iterating until 0
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Validated: partial, pass, 105 terms through n = 104 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static idx_t A324920_split(idx_t n)
{
    /* largest divisor d <= sqrt(n); return n/d - d */
    idx_t lo = 0, hi = (idx_t)1 << 32, s = 0;
    if (hi > n) hi = n;
    while (lo <= hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid <= n / (mid ? mid : 1) || mid == 0) {
            s = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    idx_t d = s;
    while (d > 1) {
        if (n % d == 0) break;
        d--;
    }
    if (d == 0) d = 1;
    return n / d - d;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n > 0) {
        n = A324920_split(n);
        count++;
    }
    return count;
}
