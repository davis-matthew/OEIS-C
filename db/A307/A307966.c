/*
 * A307966  Sum of the largest side lengths of all integer-sided triangles
 *          with perimeter n.
 * https://oeis.org/A307966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{c=ceil(n/3)}^{floor((n-1)/2)} c * count(c), where
 *            count(c) = max(0, floor((n-c)/2) - max(1, n-2c) + 1). "count(c)"
 *            is the number of integer pairs (a,b) with a ≤ b ≤ c, a+b=n-c and
 *            a+b>c. This sums the largest side length c over all
 *            integer-sided triangles with perimeter n
 * Range:     u8 n <= 28, u16 n <= 190, u32 n <= 7740, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* ceil(n/3) = (n+2)/3, floor((n-1)/2) = (n-1)/2 */
    idx_t start = (n + 2) / 3;
    idx_t end   = (n - 1) / 2;

    for (idx_t c = start; c <= end; ++c) {
        seq_t c_seq = (seq_t)c;
        seq_t s = (seq_t)n - c_seq;              /* a + b */
        seq_t a_lo = (s > c_seq) ? (s - c_seq) : (seq_t)1;   /* max(1, s - c) */
        seq_t a_hi = s / 2;                         /* floor(s/2) */
        if (a_hi < a_lo) continue;                  /* no valid (a,b) */
        seq_t count = a_hi - a_lo + 1;
        sum += c_seq * count;
    }
    return sum;
}
