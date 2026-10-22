/*
 * A336302  a(n) = n^2 mod ceiling(sqrt(n)).
 * https://oeis.org/A336302
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, look
 * Formula:   a(n) = n^2 mod ceil(sqrt(n))
 * Range:     u8 n <= 373, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute ceil(sqrt(n)) for n up to 2^64-1 */
static uint64_t ceil_sqrt_uint64(uint64_t n)
{
    if (n == 0) return 0;
    uint64_t low = 0;
    uint64_t high = ((uint64_t)1 << 32); /* sqrt(2^64-1) < 2^32 */

    while (low < high) {
        uint64_t mid = low + ((high - low) >> 1);
        /* mid*mid fits in 64 bits because mid < 2^32 */
        uint64_t sq = mid * mid;
        if (sq < n)
            low = mid + 1;
        else
            high = mid;
    }
    return low;
}

seq_t a(idx_t n)
{
    /* Notice: offset is 1, so valid n >= 1.  n==0 handled trivially. */
    uint64_t s = ceil_sqrt_uint64((uint64_t)n);
    seq_t nn = (seq_t)n;
    seq_t res = (nn * nn) % (seq_t)s;
    return res;
}
