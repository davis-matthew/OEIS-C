/*
 * A350178  Take n and subtract the greatest square less than or equal to n.
 *          Repeat this process until 0 is reached. a(n) is the sum of all
 *          residues after subtractions.
 * https://oeis.org/A350178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, hear
 * Formula:   a(n) = n - r^2 + a(n - r^2), r = floor(sqrt(n)) (greedy
 *            subtraction of largest square)
 * Range:     u8 n <= 5000, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t isqrt_u64(idx_t n)
{
    if (n < 2) return n;
    idx_t low = 0;
    idx_t high = 1ULL << 32;  /* sqrt(2^64-1) < 2^32 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (mid > n / mid)  /* mid*mid > n without overflow */
            high = mid - 1;
        else
            low = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    while (n > 0) {
        idx_t r   = isqrt_u64(n);
        idx_t sq  = r * r;
        idx_t res = n - sq;
        sum += (seq_t)res;
        n = res;
    }
    return sum;
}
