/*
 * A399840  a(n) = A050443(n) mod n.
 * https://oeis.org/A399840
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A050443(n) mod n, where A050443(0)=4, A050443(1)=0,
 *            A050443(2)=0, A050443(3)=3, A050443(k) = A050443(k-3) +
 *            A050443(k-4); the recurrence is run modulo n
 * Range:     u8 n <= 274, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 95 terms through n = 9889 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    uint64_t m = (uint64_t)n;
    uint64_t buf[4];
    buf[0] = 4 % m;
    buf[1] = 0;
    buf[2] = 0;
    buf[3] = 3 % m;
    if (n < 4) return (seq_t)buf[n];
    for (uint64_t i = 4; i <= (uint64_t)n; ++i) {
        uint64_t p = buf[(i - 3) & 3];
        uint64_t q = buf[(i - 4) & 3];
        uint64_t s = p + q;
        if (s < p || s >= m) s -= m;
        buf[i & 3] = s;
    }
    return (seq_t)buf[n & 3];
}
