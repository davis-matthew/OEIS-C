/*
 * A342774  Length of n-th word in the ordering A342753 of all binary words.
 * https://oeis.org/A342774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(c(n)), a(0) = 0, where c(n) = floor((n-1)/3) if
 *            3|n, else (n-1) - floor((n-1)/3) (word-length recursion for
 *            A342753)
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Validated: partial, pass, 86 terms through n = 86 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t m = n;
    idx_t cnt = 0;
    while (m > 0) {
        idx_t t = (m - 1) / 3;
        idx_t s = (m - 1) - t;
        m = (m % 3 == 0) ? t : s;
        cnt++;
    }
    return (seq_t)cnt;
}
