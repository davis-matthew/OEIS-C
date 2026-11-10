/*
 * A286576  a(n) = A132971(n) mod 3.
 * https://oeis.org/A286576
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = A132971(n) mod 3, where A132971(0) = 1, A132971(2n) =
 *            A132971(n), A132971(4n+1) = -A132971(n), A132971(4n+3) = 0
 * Range:     u8 n <= 10922, u16 n <= 10922, u32 n <= 10922, u64 n <= 10922,
 *            u128 n <= 10922, big any n
 * Validated: standard, pass, 117 terms through n = 9547 (2026-11-08)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static int A132971_sign(idx_t n)
{
    int sign = 1;
    while (n != 0) {
        if ((n & 1ULL) == 0) {
            n >>= 1;
            continue;
        }
        if ((n & 3ULL) == 1) {
            sign = -sign;
            n = (n - 1) >> 2;
        } else {
            return 0;
        }
    }
    return sign;
}

seq_t a(idx_t n)
{
    int s = A132971_sign(n);
    if (s == 0) return (seq_t)0;
    if (s > 0) return (seq_t)1;
    return (seq_t)2;
}
