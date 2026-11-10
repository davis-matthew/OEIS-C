/*
 * A373260  a(n) = 1 if A276085(n) == +1 (mod 3), otherwise 0, where A276085
 *          is the primorial base log-function.
 * https://oeis.org/A373260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (v2(n) + 2*v3(n)) mod 3 == 1, else 0; this holds
 *            because A276085(n) = Sum_{p^e || n} e*(product of primes < p),
 *            and that product is 1 for p=2, 2 for p=3, and divisible by 3 for
 *            every p >= 5
 * Range:     u8 n <= 100000, u16 n <= 100000, u32 n <= 100000,
 *            u64 n <= 100000, u128 n <= 100000, big any n
 * Validated: standard, pass, 137 terms through n = 9844 (2026-11-10)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    unsigned long long e2 = 0, e3 = 0;
    idx_t m = n;
    if (m == 0) return (seq_t)0;
    while ((m & 1) == 0) { m >>= 1; e2++; }
    while (m % 3 == 0) { m /= 3; e3++; }
    unsigned long long r = (e2 + 2 * e3) % 3;
    return (seq_t)(r == 1 ? 1 : 0);
}
