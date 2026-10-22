/*
 * A358854  Number of even digits necessary to write all the numbers from 0 up
 *          to n.
 * https://oeis.org/A358854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Sum_{k=0..n} (# even digits of k)
 * Range:     u8 n <= 229, u16 n <= 9508, u32 n <= 9508, u64 n <= 9508,
 *            u128 n <= 9508, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Count the number of even decimal digits in a non‑negative integer */
static int count_even_digits(uint64_t x)
{
    int cnt = 0;
    if (x == 0) {
        /* 0 itself is an even digit */
        return 1;
    }
    while (x > 0) {
        int d = x % 10;
        if ((d & 1) == 0) {
            ++cnt;
        }
        x /= 10;
    }
    return cnt;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t i = 0; i <= n; ++i) {
        uint64_t val = (uint64_t)i;
        int cnt = count_even_digits(val);
        sum += (seq_t)cnt;
    }
    return sum;
}
